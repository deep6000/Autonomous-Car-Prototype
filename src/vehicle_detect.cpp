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
sem_t sem_vehicle,sem_ped;

void* vehicle_detect(void* threadargs)
{
    while(command == RUN)
    {
    Mat frame = Cap_frame.clone();
    frame = Cropframe(frame,2);
    frame_locs.car_cord = GetVehicleCoordinates(frame);
    if(command == STOP)
        break;
    sem_post(&sem_vehicle);
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
    car_cascade.detectMultiScale(frame,detected_cars, 1.2, 3, CASCADE_DO_CANNY_PRUNING, Size(0, 0));	
    return detected_cars;
}

Mat Cropframe(Mat input, uint8_t factor)
{
	Mat result;
	result = input( Rect( 0, input.rows/factor, input.cols, input.rows/factor));
	return result;
}