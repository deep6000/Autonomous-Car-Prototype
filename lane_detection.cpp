/**
 * @file lane_detection.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-08-02
 * @reference https://github.com/MichiMaestre/Lane-Detection-for-Autonomous-Cars/blob/master/LaneDetector/demo.cpp
 * @copyright Copyright (c) 2019
 * 
 */
#include "lane_detection.h"





int main(int argc, char** argv)
{
	VideoCapture capture("02260002(1).AVI");
	
	Mat frame,frame_half,framebr,HLS,HSV,yellow,white,denoise,edge,lanes, lane_detect,roi_mask;

	vector<vector<Vec4i>> left_right_lines;
	std::vector<cv::Point> lane;
	Point l; 
	
	if(!capture.isOpened())
	{
		cout << "Cannot open camera." << endl;
		return 0;
	}
	while(1)
	{
		//frame = imread(argv[1]);
		capture >> frame;
		//imwrite("test.png",frame);
		
		//frame_half = Cropframe(frame,2);
		framebr =frame;

		ConvertRGB2HLS(framebr,HLS);
		//imshow("HLS",HLS);

		
		ConvertRGB2HSV(framebr, HSV);
		
		//imshow("HSV",HSV);
		
		ExtractWhite(HLS,white);
		//imshow("white",white);

		ExtractYellow(HSV,yellow)	
		//imshow("HLSyellow",yellow);
		
		OR_Frames(white, yellow, lanes);
		//imshow("ORing",lanes);
		denoise = DenoiseFrame(lanes,KERNEL_SIZE);
		
		//imshow("Denoise",denoise);

		edge = DetectEdge(denoise,CANNY_MIN_THRES,CANNY_MAX_THRES);
		//imshow("Edge",edge);

	    roi_mask = CreateROImask(edge);
	//	imshow("ROI MASK", roi_mask);
		
		AND_Frames(edge, roi_mask, edge);
		//imshow("Edge", edge);
		vector<Vec4i> lines;


		HoughLinesP(edge, lines, 1, CV_PI/180, 40, 10, 100);
	
		left_right_lines = LaneSeperation(lines, edge);
	
	//	lane = AverageLines(left_right_lines, edge);

		//frame = PlotLines(frame,lane);
		frame = PlotLines2(frame,left_right_lines);
		imshow("OUTPUT", frame);

		
		waitKey(1);
	}
}

