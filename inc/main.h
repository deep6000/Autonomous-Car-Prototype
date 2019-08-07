#ifndef MAIN_H
#define MAIN_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect.hpp"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <pthread.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <signal.h>
#include "common.h"




using namespace cv;
using namespace std;

#define NUM_OF_CORES        (2)
#define NUM_OF_THREADS      (2)

cpu_set_t cpuset[NUM_OF_CORES];

pthread_t threads[NUM_OF_THREADS];
pthread_attr_t sched_attr[NUM_OF_THREADS];


typedef struct ThreadArgs
{
    int i;
}ThreadArgs_t;

ThreadArgs_t threadargs[NUM_OF_THREADS];


/**
 * @brief Detect Lanes on the frame
 * 
 */
void DetectLanes();


#endif /* MAIN_H */
