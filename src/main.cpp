
/**
 * @file main.cpp
 * @author Deepesh Sonigra
 * @brief 
 * @version 0.1
 * @date 2019-08-06
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "main.h"
#include "lane_detection.h"
#include "vehicle_detect.h"
#include "signal_detect.h"
#include "time_cal.h"
#include "errno.h"

Mat Cap_frame;

FramePts frame_locs;

Status command;

//https://github.com/abhi-kumar/CAR-DETECTION
//https://github.com/anhydrous99/CarDetection
String ped_cascade_name = "../pedestrian.xml";
String car_cascade_name = "../cascade2.xml";
sem_t sem_main;



int main(int argc, char** argv)
{
    command = RUN;
    struct timespec start, finish, diff;
    unsigned int framecount = 0 ;

    int scope;
    Mat Cap;

    VideoCapture cap(argv[1]);
     if(!cap.isOpened())
    {
        cout << "Error opening input video stream or file" << endl;
        return -1;
    }
   

    car_cascade_name = string(argv[2]);
    if(!(LoadCascade(car_cascade_name)))
    {
        cout<< "Error opening  Car Cascade Classifier"<<endl; 
    }
    if(!(LoadSignalCascade(ped_cascade_name)))
    {
        cout<< "Error opening Signal Cascade Classifier"<<endl; 
    }
     CreateSemaphores();


    cap >> Cap;
    resize(Cap,Cap_frame,Size(Cap.cols/2, Cap.rows/2));
     VideoWriter output;
    output.open("output.avi", CV_FOURCC('M','P','4','V'), cap.get(CV_CAP_PROP_FPS), Size(Cap_frame.cols, Cap_frame.rows ), true);
    if(!output.isOpened())
    {
        cout << "Error opening output video stream or file" << endl;
        return -1;
    }

    pthread_create(&threads[0],(pthread_attr_t*)(&sched_attr[0]),lane_detection ,(void *) &(threadargs[0]));
    pthread_create(&threads[1],(pthread_attr_t*)(&sched_attr[1]),vehicle_detect ,(void *) &(threadargs[1]));
    pthread_create(&threads[2],(pthread_attr_t*)(&sched_attr[3]),signal_detect ,(void *) &(threadargs[2]));

    clock_gettime(CLOCK_REALTIME, &start);
    while(command == RUN)
    {
    
        cap >> Cap;
           if(Cap.empty())
              break; 
        resize(Cap,Cap_frame,Size(Cap.cols/2, Cap.rows/2));
        framecount++;
     
        if(framecount % 1 == 0)
            sem_post(&sem_lane);
         if(framecount % 1== 0)
            sem_post(&sem_vehicle);
         if(framecount % 10 == 0)
            sem_post(&sem_signal);

        char k = waitKey(5);
        if(k == 27)
        {
           break;
        }
      
        DetectLanes();
        DetectCars();
        DetectSignal();
      
        imshow("Output",Cap_frame);
        output.write(Cap_frame);



    }
    clock_gettime(CLOCK_REALTIME, &finish);

    sem_post(&sem_lane);
    sem_post(&sem_vehicle);
    sem_post(&sem_signal);
    DestroySemaphores();

    destroyAllWindows();
    command = STOP;

    for(int i=0;i<NUM_OF_THREADS;i++)
		pthread_join(threads[i], NULL);
    delta_t(&finish,&start,&diff);
    printf("Total Time spent is %ld\n", diff.tv_sec);
    printf("Total Frames Processed %d \n",framecount);
    float frame_rate = (float)(framecount/(diff.tv_sec));
     printf("Frame Rate = %02f\n",frame_rate);
}

void DetectLanes()
{

        if(frame_locs.lane.left_lane)
        {
         
            Vec4i left_final = frame_locs.lane.left_lane_pts;
            left_final[1] += Cap_frame.rows*0.5;
            left_final[3] += Cap_frame.rows*0.5;
            line(Cap_frame, Point(left_final[0], left_final[1]), Point(left_final[2], left_final[3]), Scalar(0,0,255), 3, CV_AA);
        }
       
        if(frame_locs.lane.right_lane)
        {   
           
            Vec4i right_final = frame_locs.lane.right_lane_pts;
            right_final[1] += Cap_frame.rows*0.5;
            right_final[3] += Cap_frame.rows*0.5;
            line(Cap_frame, Point(right_final[0], right_final[1]), Point(right_final[2], right_final[3]), Scalar(0,0,255), 3, CV_AA);
        } 
    
        
}

void DetectCars()
{
   
    for(size_t i = 0; i< frame_locs.car_cord.size(); i++)
    {
        Rect r = frame_locs.car_cord[i];
        r.y = r.y+ (0.4* Cap_frame.rows);
       
        rectangle(Cap_frame,r,Scalar(255,0,255), 2, 8, 0);
    
    }

}

void DetectSignal()
{
    for(size_t i = 0; i< frame_locs.signal_cord.size(); i++)
    {
        Rect r = frame_locs.signal_cord[i];
     
        rectangle(Cap_frame,r,Scalar(255,0,0), 2, 8, 0);
    
    }

}

void CreateSemaphores()
{
    if(sem_init(&sem_main, 0, 0) == -1)
        printf("Error seminit\n");

    if(sem_init(&sem_lane, 0, 0) == -1)
        printf("Error seminit\n"); 

    if(sem_init(&sem_signal, 0, 0) == -1)
        printf("Error seminit\n");

    if(sem_init(&sem_vehicle, 0, 0) == -1)
        printf("Error seminit\n");
}

void DestroySemaphores()
{
    sem_destroy(&sem_main);
    sem_destroy(&sem_signal);
    sem_destroy(&sem_vehicle);
}
