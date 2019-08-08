
#ifndef COMMON_H
#define COMMON_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect.hpp"

using namespace cv;
using namespace std;


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

#endif