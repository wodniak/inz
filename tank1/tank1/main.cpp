#include<opencv2/opencv.hpp>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tank.h"
#include "myimgproc.h"

using namespace std;
using namespace cv;


//virtual bluetooth com port
char port_name[9] = "\\\\.\\COM8";

int main()
{
	/*
	cout << "Started program.\n";
	Tank tank(port_name);
	if (tank.isConnected())
		cout << "Connection Established" << endl;
	else
		cout << "ERROR, check port name";
	while (tank.isConnected())
	{

		tank.steer();
		Sleep(500);
		tank.read_sensors();
	}
	*/
	Myimgproc::createAllWindows();

	/* 
	 * Create a VideoCapture object and open the input file
	 */ 
	VideoCapture cap("films\\MAH00922.mp4");

	// Check if camera opened successfully
	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return -1;
	}	
	
	//matrix for each frame in video
	Mat frame;

	//get first frame without robot
	cap >> frame;

	Myimgproc::init();

	//get maze
	Myimgproc::draw_maze(frame);

	int i = 0;
	//loop until end of video
	while (!frame.empty())
	{
		cout << i;
		i++;
		//keep track of tank
		Myimgproc::processImages(frame);
		
		cap >> frame;

		//wait between frames
		waitKey(100);

	}
	
	return 0;
}