#pragma once
#ifndef MYIMGPROC_H
#define MYIMGPROC_H

#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<opencv2/opencv.hpp>

void processImages(const char* firstFrameFilename);

#endif // MYIMGPROC_H