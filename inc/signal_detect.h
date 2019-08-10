//https://github.com/AdityaPai2398/Vehicle-And-Pedestrian-Detection-Using-Haar-Cascades/tree/master/Main%20Project/Main%20Project/Pedestrian%20Detection


 
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
 * @brief 
 * 
 * @param threadargs 
 * @return void* 
 */
void* signal_detect(void* threadargs);

/**
 * @brief Get the Ped Coordinates object
 * 
 * @param frame 
 * @return vector<Rect> 
 */

vector<Rect> GetSignalCoordinates(Mat frame);
/**
 * @brief 
 * 
 * @param input 
 * @return Mat 
 */
Mat CropSignalframe(Mat input);

/**
 * @brief 
 * 
 * @param cascade_name 
 * @return uint8_t 
 */
uint8_t LoadSignalCascade(String cascade_name);