
#ifndef COMMON_H
#define COMMON_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include<semaphore.h>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect.hpp"

using namespace cv;
using namespace std;

#define ConvertRGB2HLS(input,output) 			cvtColor(input, output, COLOR_BGR2HLS);
#define ConvertRGB2HSV(input,output) 			cvtColor(input, output, COLOR_BGR2HSV);
#define ConvertRGB2GRAY(input,output) 			cvtColor(input, output, COLOR_BGR2GRAY);


typedef struct Lane_Cordinates
{
    Vec4i right_lane_pts;
    Vec4i left_lane_pts;
    bool right_lane;
    bool left_lane;
}Lane_Cordinates;


typedef struct FramePts
{
  Lane_Cordinates lane;
  vector<Rect> car_cord;
} FramePts;

typedef enum Status
{
    STOP,
    RUN
}Status;

extern Status command;

extern Mat Cap_frame;

extern FramePts frame_locs;

extern CascadeClassifier car_cascade;

extern sem_t sem_main, sem_lane,sem_vehicle, sem_ped;

#endif