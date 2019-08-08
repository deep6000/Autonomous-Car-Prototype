#ifndef VEHICLE_DETECT_H
#define VEHICLE_DETECT_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "common.h"


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect.hpp"

using namespace cv;
using namespace std;


/**
 * @brief  Vehicle Detect callback function
 * 
 * @param threadargs 
 * @return void* 
 */
void* vehicle_detect(void* threadargs);

/**
 * @brief Loading car cascade 
 * 
 * @param cascade_name 
 * @return uint8_t 
 */
uint8_t LoadCascade(String cascade_name);

/**
 * @brief Get the Vehicle Coordinates object
 * 
 * @param frame 
 * @return vector<Rect> 
 */
vector<Rect> GetVehicleCoordinates(Mat frame);



/**
 * @brief  Crop image which you don't need
 * 
 * @param input 
 * @param factor 
 * @return Mat 
 */
Mat Cropframe(Mat input, uint8_t factor);


#endif /* VEHICLE_DETECT_H */
