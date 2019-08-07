
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

Mat Cap_frame;
FramePts frame_locs;

int main(int argc, char** argv)
{

    VideoCapture cap("/home/deepesh/Examples_OpenCV/project/input.AVI");
     cap >> Cap_frame;
     if(!cap.isOpened())
    {
        cout << "Error opening input video stream or file" << endl;
        return -1;
    }

    for(int core = 0; core < NUM_OF_CORES; core ++)
        CPU_ZERO(&cpuset[core]);

    for(int core = 0 ; core< NUM_OF_CORES; core++)
    {
        CPU_SET(core, &cpuset[core]);
        pthread_attr_init(&sched_attr[core]);
        pthread_attr_setaffinity_np(&sched_attr[core],sizeof(cpuset[core]), &cpuset[core]);
        
    }
    
    pthread_create(&threads[0],(pthread_attr_t*)(&sched_attr[0]),lane_detection ,(void *) &(threadargs[0]));


    while(1)
    {

        cap >> Cap_frame;
        if(Cap_frame.empty())
        {
            cout<<"Empty"<<endl;
        }

        char k = waitKey(10);
        if(k == 27)
            break;

        DetectLanes();
 

        imshow("Output",Cap_frame);

    }
}




void DetectLanes()
{
   
        if(frame_locs.lane.left_lane)
        {
            Vec4i left_final = frame_locs.lane.left_lane_pts;
            line(Cap_frame, Point(left_final[0], left_final[1]), Point(left_final[2], left_final[3]), Scalar(0,0,255), 3, CV_AA);
        }
       
        if(frame_locs.lane.right_lane)
        {
            Vec4i right_lane = frame_locs.lane.right_lane_pts;
            line(Cap_frame, Point(right_lane[0], right_lane[1]), Point(right_lane[2], right_lane[3]), Scalar(0,0,255), 3, CV_AA);
        } 
        
}
