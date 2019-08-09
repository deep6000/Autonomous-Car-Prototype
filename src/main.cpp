
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
#include "time_cal.h"

Mat Cap_frame;

FramePts frame_locs;

Status command;

//https://github.com/abhi-kumar/CAR-DETECTION
//https://github.com/anhydrous99/CarDetection
String car_cascade_name = "../cascade2.xml";
sem_t sem_main;



int main(int argc, char** argv)
{
    command = RUN;
    struct timespec start, finish, diff;
    unsigned int framecount = 0 ;

    int scope;
	pthread_attr_getscope(&main_attr, &scope);
 	if(scope == PTHREAD_SCOPE_SYSTEM)
    		printf("PTHREAD SCOPE SYSTEM\n");
  	else if (scope == PTHREAD_SCOPE_PROCESS)
    		printf("PTHREAD SCOPE PROCESS\n");
	else
	    	printf("PTHREAD SCOPE UNKNOWN\n");

    VideoCapture cap(argv[1]);
     if(!cap.isOpened())
    {
        cout << "Error opening input video stream or file" << endl;
        return -1;
    }

    car_cascade_name = string(argv[2]);
    if(!(LoadCascade(car_cascade_name)))
    {
        cout<< "Error opening Cascade Classifier"<<endl; 
    }
    
     CreateSemaphores();
    // Cap_frame = imread(argv[1]);
    cap >> Cap_frame;
  
    
    // for(int core = 0; core < NUM_OF_CORES; core ++)
    //     CPU_ZERO(&cpuset[core]);

    // for(int core = 0 ; core< NUM_OF_CORES; core++)
    // {
    //     CPU_SET(core, &cpuset[core]);
    //     pthread_attr_init(&sched_attr[core]);
    //     pthread_attr_setaffinity_np(&sched_attr[core],sizeof(cpuset[core]), &cpuset[core]);
        
    // }



    max_prio = sched_get_priority_max(SCHED_FIFO);
	min_prio = sched_get_priority_min(SCHED_FIFO);

    struct sched_param main;
	main.sched_priority = max_prio;
	int ret = sched_setscheduler(getpid(), SCHED_FIFO, &main);
	if (ret < 0) 
	{
		printf("Unsuccessful in setting thread realtime prio\n");
		return 1;     
	}

    for(int i = 0 ; i < NUM_OF_THREADS; i++)
    {
        pthread_attr_init(&sched_attr[i]);
	    pthread_attr_setinheritsched(&sched_attr[i], PTHREAD_EXPLICIT_SCHED);
	    pthread_attr_setschedpolicy(&sched_attr[i], SCHED_FIFO);
	    pthread_attr_setscope(&sched_attr[i], PTHREAD_SCOPE_SYSTEM);
    }

    rt_param[0].sched_priority = max_prio -1;
    pthread_attr_setschedparam(&sched_attr[0], &rt_param[0]);

    rt_param[1].sched_priority = max_prio -2;
    pthread_attr_setschedparam(&sched_attr[1], &rt_param[1]);

    pthread_create(&threads[0],(pthread_attr_t*)(&sched_attr[0]),lane_detection ,(void *) &(threadargs[0]));
    pthread_create(&threads[1],(pthread_attr_t*)(&sched_attr[1]),vehicle_detect ,(void *) &(threadargs[1]));


    clock_gettime(CLOCK_REALTIME, &start);
    while(command == RUN)
    {
        
        cap >> Cap_frame;
        if(Cap_frame.empty())
              break;
    
        framecount++;
        char k = waitKey(1);
        if(k == 27)
           break;
        sem_wait(&sem_lane);
        DetectLanes();
        sem_wait(&sem_vehicle);
        DetectCars();

        imshow("Output",Cap_frame);


    }
    clock_gettime(CLOCK_REALTIME, &finish);
    destroyAllWindows();
    command = STOP;
    for(int i=0;i<NUM_OF_THREADS;i++)
		pthread_join(threads[i], NULL);
    DestroySemaphores();
    delta_t(&finish,&start,&diff);
    printf("Total Time spent is %ld\n", diff.tv_sec);
    printf("Total Frames Processed %d \n",framecount);
    double frame_rate = framecount/(diff.tv_sec);
     printf("Frame Rate = %d\n",(int)frame_rate);
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

void CreateSemaphores()
{
    if(sem_init(&sem_main, 0, 0) == -1)
        printf("Error seminit\n");

    if(sem_init(&sem_lane, 0, 0) == -1)
        printf("Error seminit\n"); 

    if(sem_init(&sem_ped, 0, 0) == -1)
        printf("Error seminit\n");

    if(sem_init(&sem_vehicle, 0, 0) == -1)
        printf("Error seminit\n");
}

void DestroySemaphores()
{
    sem_destroy(&sem_main);
    sem_destroy(&sem_ped);
    sem_destroy(&sem_vehicle);
}