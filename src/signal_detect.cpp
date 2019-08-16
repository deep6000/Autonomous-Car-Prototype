
#include "signal_detect.h"




CascadeClassifier ped_cascade;
sem_t sem_signal,sem_signal_d;


void* signal_detect(void* threadargs)
{
  
 
    Mat frame , gray;
    while(command == RUN)
    {
     
       sem_wait(&sem_signal);
     
        frame = Cap_frame.clone();
        frame = CropSignalframe(frame);
      
        ConvertRGB2GRAY(frame,gray);
        frame_locs.signal_cord = GetSignalCoordinates(gray);
        if(command == STOP)
            break;
    
    
    }
   
    cout<<"Exiting Signal Detection"<<endl;
    pthread_exit(NULL);

}

vector<Rect> GetSignalCoordinates(Mat frame)
{
    vector<Rect> detected_peds;
    ped_cascade.detectMultiScale(frame,detected_peds, 1.2, 3, CASCADE_DO_CANNY_PRUNING, Size(0, 0));	
    return detected_peds;
}

Mat CropSignalframe(Mat input)
{
	Mat result;
	result = input( Rect( 0, 0, input.cols, input.rows*0.5));
	return result;
}


uint8_t LoadSignalCascade(String cascade_name)
{
    if(! ped_cascade.load(cascade_name))
    {
        cout<< "Fail to load the ped cascade"<<endl;   
        return 0;
    }
    return 1;
}
