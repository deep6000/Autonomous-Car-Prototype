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

double right_m, left_m;
Point right_b,left_b;
bool right_lane , left_lane;

void* lane_detection(void* threadargs)
{

	Mat frame,HLS,HSV,yellow,white,denoise,edge,lanes, lane_detect,roi_mask;

	vector<vector<Vec4i>> left_right_lines;
	std::vector<cv::Point> lane;
	Point l; 
	
	while(command == RUN)
	{
		//frame = imread(argv[1]);
		frame = Cap_frame.clone();

	
		//imwrite("test.png",frame);
		
		ConvertRGB2HLS(frame,HLS);
		//imshow("HLS",HLS);
		//imwrite("HLS.png",HLS);
		
		ConvertRGB2HSV(frame, HSV);
		//imwrite("HSV.png",HSV);
		//imshow("HSV",HSV);
		
		ExtractWhite(HLS,white);
		//imwrite("white.png",white);
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
		//imshow("ROI MASK", roi_mask);
		
		AND_Frames(edge, roi_mask, edge);
		//imshow("Edge", edge);

		vector<Vec4i> lines;
		HOUGH_LINES(edge,lines);
		
		left_right_lines = LaneSeperation(lines, edge);

		//add semaphores here
		frame_locs.lane = GetLinesCordinates(frame,left_right_lines);
			
	}
	cout<<"Exiting Lane Detection"<<endl;
	pthread_exit(NULL);

}

double average(int a[], int n) 
{ 
    // Find sum of array element 
    int sum = 0; 
    for (int i=0; i<n; i++) 
       sum += a[i]; 
  
    return sum/n; 
} 


Mat Cropframe(Mat input, uint8_t factor)
{
	Mat result;
	result = input( Rect( 0, input.rows/factor, input.cols, input.rows/factor));
	return result;
}


Mat DenoiseFrame(Mat input, uint8_t kernel_size)
{
	Mat gaussian;
	GaussianBlur(input, gaussian, Size(kernel_size, kernel_size), 0, 0 );
	return gaussian;
} 


Mat DetectEdge(Mat input,unsigned int MinThresh, unsigned int MaxThresh)
{
	Mat Edge;
	Canny(input, Edge, MinThresh, MaxThresh, 3, true);	
	return Edge;
}

Mat CreateROImask(Mat input)
{
		Mat mask;
				//Add ROI function here
		Point mask_pts[1][4];
		int sides = 4;
		mask = Mat::zeros(Size(input.cols, input.rows), CV_8U);
		//Points for ROI mask
		mask_pts[0][0] = Point(input.cols*0.4,input.rows* 0.6);					
		mask_pts[0][1] = Point(input.cols*0.3, input.rows*0.8);
		//mask_pts[0][2] = Point(0, input.rows);						
		mask_pts[0][2] = Point(input.cols*0.8, input.rows*0.8);	
		mask_pts[0][3] = Point(input.cols*0.5, input.rows*0.6);					
		const Point* pts_list[1] = {mask_pts[0]};
		fillPoly(mask, pts_list, &sides, 1, 255, 8);
		return mask;

}

Mat BrightenFrame(Mat image)
{
	Mat new_image;
 	for( int y = 0; y < image.rows; y++ )
    {
		for( int x = 0; x < image.cols; x++ )
        {
			  for( int c = 0; c < 3; c++ )
              {
      			image.at<Vec3b>(y,x)[c] =saturate_cast<uchar>( 1.5*( image.at<Vec3b>(y,x)[c] ) + 100 );
              }
    	}
    }
	return image;
}


vector<vector<Vec4i>> LaneSeperation(vector <Vec4i> lines, Mat frame)
{
	double slope;
	vector<vector<Vec4i> > output(2);
	int j = 0, k = 0;
	vector<Vec4i> right_lanes, left_lanes;
	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];

		slope = (l[3]-l[1])/((double)(l[2]-l[0]));				//calculate slope to remove almost horizontal lines from this list
		if(slope>0.5 || slope<(-0.5))	
		{
			
			if(l[2] > frame.cols/2 || l[0] > frame.cols/2)
			{
				right_lane = true;
				right_lanes.push_back(lines[i]);
				
			}
			else
			{
				left_lane = true;
				left_lanes.push_back(lines[i]);
			
			}
		}
	}           
	output[0] = right_lanes;
  	output[1] = left_lanes;
	return output;

}

vector<Point> AverageLines(vector<vector<Vec4i> > left_right_lines, Mat inputImage)
{
	Point ini1,fini1, ini2,fini2;
	vector<Point> right_pts;
	vector<Point> left_pts;
    Vec4d right_line;
    Vec4d left_line;
    
    vector<Point> output(4);

		if(right_lane)
		{
			for(auto i : left_right_lines[0])
			{
				ini1 = Point(i[0],i[1]);
				fini1 = Point(i[2],i[3]);
				
				right_pts.push_back(ini1);
				right_pts.push_back(fini1);
			}
			if(right_pts.size() > 0)
			{
				fitLine(right_pts,right_line,CV_DIST_L2,0,0.01,0.01);
				right_m = right_line[1] / right_line[0];
				right_b = Point(right_line[2], right_line[3]);
			}
        
		}
        if (left_lane == true) 
        {
            for (auto j : left_right_lines[1]) 
            {
                ini2 = cv::Point(j[0], j[1]);
                fini2 = cv::Point(j[2], j[3]);

                left_pts.push_back(ini2);
                left_pts.push_back(fini2);
            }

            if (left_pts.size() > 0) 
            {
    
                fitLine(left_pts, left_line, CV_DIST_L2, 0, 0.01, 0.01);
                left_m = left_line[1] / left_line[0];
                left_b = cv::Point(left_line[2], left_line[3]);
            }
        }
        int ini_y = 0.8  * inputImage.rows;
        int fin_y = 0.65 * inputImage.rows;

        double right_ini_x = ((ini_y - right_b.y) / right_m) + right_b.x;
        double right_fin_x = ((fin_y - right_b.y) / right_m) + right_b.x;

        double left_ini_x = ((ini_y - left_b.y) / left_m) + left_b.x;
        double left_fin_x = ((fin_y - left_b.y) / left_m) + left_b.x;

        output[0] = Point(right_ini_x, ini_y);
        output[1] = Point(right_fin_x, fin_y);
        output[2] = Point(left_ini_x, ini_y);
        output[3] = Point(left_fin_x, fin_y);

        return output;

}


Mat PlotLines(Mat inputImage, vector<Point> lane)
{
  vector<Point> poly_points;
  Mat output;

//   inputImage.copyTo(output);
//   poly_points.push_back(lane[2]);
//   poly_points.push_back(lane[0]);
//   poly_points.push_back(lane[1]);
//   poly_points.push_back(lane[3]);
//   fillConvexPoly(output, poly_points, Scalar(0, 0, 255), CV_AA, 0);
 // addWeighted(output, 0.3, inputImage, 1.0 - 0.3, 0, inputImage);
  double slope1  = (lane[1].y - lane[0].y) - (lane[1].x - lane[0].x);
  double slope2  = (lane[3].y - lane[2].y) - (lane[3].x - lane[2].x);

  if(slope1 > 0.5 || slope1 <(-0.5))
  		line(inputImage, lane[0], lane[1], Scalar(0, 255, 255), 5, CV_AA);
  if(slope2 > 0.5 || slope2 <(-0.5))
  		line(inputImage, lane[2], lane[3], Scalar(0, 255, 255), 5, CV_AA);

  return inputImage;

}


Lane_Cordinates GetLinesCordinates( Mat frame, vector<vector<Vec4i> > left_right_lines)
{
	Lane_Cordinates lanes;
	Mat Output;
	frame.copyTo(Output);

	vector<Vec4i> right;
	vector<Vec4i> left;

	bool right_lane_exist;
	bool left_lane_exist;
	right = left_right_lines[0];
	left  = left_right_lines[1];
	double slope;
	int UpperX[right.size()];
	int UpperY[right.size()];
	int LowerX[right.size()];
	int LowerY[right.size()];

	int LUpperX[left.size()];
	int LUpperY[left.size()];
	int LLowerX[left.size()];
	int LLowerY[left.size()];

	right_lane_exist = left_lane_exist = false;

	Vec4i right_final, left_final;

	if(right.size())
	{	
		right_lane_exist = true;
		for( size_t i = 0; i < right.size(); i++ )
		{
			Vec4i l = right[i];
			slope = (l[3]-l[1])/((double)(l[2]-l[0]));			//calculate slope to remove almost horizontal lines from this list
		
				if(slope>0.5 || slope<(-0.5))	                        //Draw only those lines which satisfy this slope criteria
				{
					UpperX[i] = l[0];
					UpperY[i] = l[1];
					LowerX[i] = l[2];
					LowerY[i] = l[3]; 
			
				}
			
		}
		right_final[0] = (int)average(UpperX, right.size());
		right_final[1] = (int)average(UpperY, right.size());
		right_final[2] = (int)average(LowerX, right.size());
		right_final[3] = (int)average(LowerY, right.size());
	
		//line(Output, Point(right_final[0], right_final[1]), Point(right_final[2], right_final[3]), Scalar(0,0,255), 3, CV_AA);	
	}
	if(left.size())
	{
		left_lane_exist = true;
		for( size_t i = 0; i < left.size(); i++ )
		{
			Vec4i l = left[i];
			slope = (l[3]-l[1])/((double)(l[2]-l[0]));				//calculate slope to remove almost horizontal lines from this list
			if(slope>0.5 || slope<(-0.5))	                        //Draw only those lines which satisfy this slope criteria
			{
				LUpperX[i] = l[0];
				LUpperY[i] = l[1];
				LLowerX[i] = l[2];
				LLowerY[i] = l[3]; 
			
				
			}
		}
			left_final[0] = average(LUpperX, left.size());
			left_final[1] = average(LUpperY, left.size()); 
			left_final[2] = average(LLowerX, left.size());
			left_final[3] = average(LLowerY, left.size());
		
		//line(Output, Point(left_final[0], left_final[1]), Point(left_final[2], left_final[3]), Scalar(0,0,255), 3, CV_AA);
	}	

	lanes.left_lane_pts = left_final;
	lanes.right_lane_pts = right_final;
	lanes.right_lane = right_lane_exist;
	lanes.left_lane = left_lane_exist;
	
	return lanes;

}
