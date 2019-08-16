/**
 * @file vehicle_detect.cpp
 * @author Deepesh Sonigra
 * @brief 
 * @version 0.1
 * @date 2019-08-06
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "vehicle_detect.h"


CascadeClassifier car_cascade;
sem_t sem_vehicle,sem_vehicle_d;

void* vehicle_detect(void* threadargs)
{
    
    Mat frame , gray;
 
    while(command == RUN)
    {
       
        sem_wait(&sem_vehicle);
        frame = Cap_frame.clone();
        frame = Cropframe(frame);
        ConvertRGB2GRAY(frame,gray);
        frame_locs.car_cord = GetVehicleCoordinates(gray);
        if(command == STOP)
            break;
      
    
    }
    	cout<<"Exiting Vehicle Detection"<<endl;
        pthread_exit(NULL);

}

uint8_t LoadCascade(String cascade_name)
{
    if(!car_cascade.load(cascade_name))
    {
        cout<< "Fail to load the car cascade"<<endl;   
        return 0;
    }
    return 1;
}


vector<Rect> GetVehicleCoordinates(Mat frame)
{
    vector<Rect> detected_cars;
    car_cascade.detectMultiScale(frame,detected_cars, 1.2, 3, 0);	
    return detected_cars;
}

Mat Cropframe(Mat input)
{
	Mat result;
	result = input( Rect( 0, input.rows*0.4, input.cols, input.rows*0.33));
	return result;
}
