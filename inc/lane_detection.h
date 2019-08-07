#ifndef LANE_DETECTION_H
#define LANE_DETECTION_H


 
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


#define KERNEL_SIZE								(7)
#define CANNY_MIN_THRES							(70)
#define CANNY_MAX_THRES							(140)
#define SLOPE_THRES								(0.5)

#define ConvertRGB2HLS(input,output) 			cvtColor(input, output, COLOR_BGR2HLS);
#define ConvertRGB2HSV(input,output) 			cvtColor(input, output, COLOR_BGR2HSV);
#define ConvertRGB2GRAY(input,output) 			cvtColor(input, output, COLOR_BGR2GRAY);

#define ExtractWhite(HSL, white)				inRange(HSL, Scalar(20,100,0), Scalar(40,255,50), white);
#define ExtractYellow(HSV, yellow)				inRange(HSV, Scalar(20,90,100), Scalar(40,255,150), yellow);

#define OR_Frames(frame1, frame2,output)		bitwise_or(frame1, frame2, output);
#define AND_Frames(frame1, frame2,output)		bitwise_and(frame1, frame2, output);

#define HOUGH_LINES(input,output)                HoughLinesP(input, output, 1, CV_PI/180, 40, 10, 100);


/**
 * @brief Lane_detection callback function
 * 
 * @param threadargs 
 * @return void* 
 */
void* lane_detection(void* threadargs);
/**
 * @brief  Find average of elements in array.
 * This code is referenced from geeksforgeeks
 * 
 * @param a 
 * @param n 
 * @return double 
 */

double average(int a[], int n);

/**
 * @brief  Crop image which you don't need
 * 
 * @param input 
 * @param factor 
 * @return Mat 
 */
Mat Cropframe(Mat input, uint8_t factor);

/**
 * @brief  Remove noise from the image
 * 
 * @param input 
 * @param kernel_size 
 * @return Mat 
 */
Mat DenoiseFrame(Mat input, uint8_t kernel_size);
/**
 * @brief Perform Edge detection
 * 
 * @param input 
 * @param MinThresh 
 * @param MaxThresh 
 * @return Mat - frame
 */

Mat DetectEdge(Mat input,unsigned int MinThresh, unsigned int MaxThresh);
/**
 * @brief Create region of interest
 * 
 * @param input 
 * @return Mat 
 */
Mat CreateROImask(Mat input);
/**
 * @brief Brighten the image 
 * 
 * @param image 
 * @return Mat 
 */
Mat BrightenFrame(Mat image);

/**
 * @brief Serperate left and right lane
 * 
 * @param lines 
 * @param frame 
 * @return vector<vector<Vec4i>> 
 */
vector<vector<Vec4i>> LaneSeperation(vector <Vec4i> lines, Mat frame);

/**
 * @brief  Find 1 line of Mulitple lines (averaging the points)
 * 
 * @param left_right_lines 
 * @param inputImage 
 * @return vector<Point> 
 */

vector<Point> AverageLines(vector<vector<Vec4i> > left_right_lines, Mat inputImage);

/**
 * @brief  PLot the lines on given points
 * 
 * @param inputImage 
 * @param lane 
 * @return Mat 
 */

Mat PlotLines(Mat inputImage, vector<Point> lane);

/**
 * @brief Plot the right and left line from the given set of lines 
 * 
 * @param frame 
 * @param left_right_lines 
 * @return Mat 
 */
Lane_Cordinates GetLinesCordinates( Mat frame, vector<vector<Vec4i> > left_right_lines);



#endif /* LANE_DETECTION_H */
