
/**
 * @file main.cpp
 * @author Deepesh Sonigra
 * @brief Autonomous Car Prototype 
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
    Mat Cap;
FramePts frame_locs;

Status command;
int test_count;
String ped_cascade_name = "../signal.xml";
String car_cascade_name = "../cars.xml";


sem_t sem_write;



int main(int argc, char** argv)
{

    command = RUN;
    struct timespec start, finish, diff;
    struct timespec Wstart, Wfinish, Wdiff;
    unsigned int framecount = 0 ;
    cpu_set_t cpu , main_cpu, write_cpu; 
    int scope;

    int i;

  
    
    CPU_ZERO(&cpu);
    CPU_ZERO(&main_cpu);
    CPU_ZERO(&write_cpu);
    CPU_SET(0, &main_cpu);
    CPU_SET(1, &main_cpu);
    CPU_SET(2, &write_cpu);
    CPU_SET(3, &write_cpu);
    char frame_name[12];

    sched_setaffinity(0,sizeof(cpu_set_t),&main_cpu);

    VideoCapture cap(argv[1]);
    if(!cap.isOpened())
    {
        cout << "Error opening input video stream or file" << endl;
        return -1;
    }
    cap >> Cap;
    framecount++;
   
    if(!(LoadCascade(car_cascade_name)))
    {
        cout<< "Error opening  Car Cascade Classifier"<<endl; 
    }

    if(!(LoadSignalCascade(ped_cascade_name)))
    {
        cout<< "Error opening Signal Cascade Classifier"<<endl; 
    }

    CreateSemaphores();

    resize(Cap,Cap_frame,Size(Cap.cols/2, Cap.rows/2));


    pthread_attr_init(&rt_sched_attr[0]);
	pthread_attr_setaffinity_np(&rt_sched_attr[0], sizeof(cpu_set_t), &main_cpu);
    
    pthread_attr_init(&rt_sched_attr[1]);
	pthread_attr_setaffinity_np(&rt_sched_attr[1], sizeof(cpu_set_t), &write_cpu);

    pthread_attr_init(&rt_sched_attr[2]);
	pthread_attr_setaffinity_np(&rt_sched_attr[2], sizeof(cpu_set_t), &write_cpu);

    pthread_attr_init(&rt_sched_attr[3]);
	pthread_attr_setaffinity_np(&rt_sched_attr[3], sizeof(cpu_set_t), &write_cpu);

    pthread_create(&threads[0],(pthread_attr_t*)&(rt_sched_attr[0]),lane_detection ,(void *) &(threadargs[0]));

    pthread_create(&threads[1],(pthread_attr_t*)&(rt_sched_attr[1]),vehicle_detect ,(void *) &(threadargs[1]));

    pthread_create(&threads[2],(pthread_attr_t*)&(rt_sched_attr[2]),signal_detect ,(void *) &(threadargs[2]));

    pthread_create(&threads[3],(pthread_attr_t*)&(rt_sched_attr[3]),write_frame ,(void *) &(threadargs[3])); 



 
    clock_gettime(CLOCK_REALTIME, &start);
    while(command == RUN)
    {
     //   clock_gettime(CLOCK_REALTIME, &Wstart);
       
     
        if(!(framecount % 4))
        {
            sem_post(&sem_lane);
            sem_post(&sem_signal);
        }
        if(!(framecount % 2))
            sem_post(&sem_vehicle);
       
        char k = waitKey(5);
        if(k == 27)
        {
           break;
        }
       
        DetectLanes();
     
        DetectCars();
    
        DetectSignal();
        
        sem_post(&sem_write);
      
        imshow("Output",Cap_frame);
      
        // clock_gettime(CLOCK_REALTIME, &Wfinish);
        // delta_t(&Wfinish,&Wstart,&Wdiff);
        // printf("Time spent  for while is %ld secs %ld nsecs\n",Wdiff.tv_sec,Wdiff.tv_nsec);
         cap >> Cap;
        if(Cap.empty())
              break; 
        resize(Cap,Cap_frame,Size(Cap.cols/2, Cap.rows/2));
        framecount++;
    }
   
    clock_gettime(CLOCK_REALTIME, &finish);

     
    command = STOP;
    sem_post(&sem_lane);
    sem_post(&sem_vehicle);
    sem_post(&sem_signal);
    sem_post(&sem_write);
    DestroySemaphores();
    destroyAllWindows();
    

    for(int i=0;i<NUM_OF_THREADS;i++)
		pthread_join(threads[i], NULL);

    delta_t(&finish,&start,&diff);
    printf("Total Time spent is %ld\n", diff.tv_sec);
    printf("Total Frames Processed %d \n",framecount);
    float frame_rate = (float)(framecount/(diff.tv_sec));
    printf("Frame Rate = %2f\n",frame_rate);
}

void DetectLanes()
{
  
        if(frame_locs.lane.left_lane )
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
       
        rectangle(Cap_frame,r,Scalar(0,255,0), 2, 8, 0);
    
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
    if(sem_init(&sem_write, 0, 0) == -1)
        printf("Error seminit\n");

    if(sem_init(&sem_lane, 0, 0) == -1)
        printf("Error seminit\n"); 

    if(sem_init(&sem_signal, 0, 0) == -1)
        printf("Error seminit\n");

    if(sem_init(&sem_vehicle, 0, 0) == -1)
        printf("Error seminit\n");

    if(sem_init(&sem_lane_d, 0, 0) == -1)
        printf("Error seminit\n"); 

    if(sem_init(&sem_signal_d, 0, 0) == -1)
        printf("Error seminit\n");

    if(sem_init(&sem_vehicle_d, 0, 0) == -1)
        printf("Error seminit\n");
}

void DestroySemaphores()
{
    sem_destroy(&sem_write);
    sem_destroy(&sem_signal);
    sem_destroy(&sem_vehicle);
    sem_destroy(&sem_lane);
    sem_destroy(&sem_signal_d);
    sem_destroy(&sem_vehicle_d);
    sem_destroy(&sem_lane_d);
}

void* write_frame(void* threadargs)
{
    
    Mat frame;
    VideoWriter output;
    output.open("output.avi", CV_FOURCC('M','P','4','V'),30 , Size(Cap.cols/2, Cap.rows/2 ), true);
    if(!output.isOpened())
    {
        cout << "Error opening output video stream or file" << endl;
    }

    while(command == RUN)
    {
        sem_wait(&sem_write);

        output.write(Cap_frame);
    }
    cout<<"Exiting write task"<<endl;

}
