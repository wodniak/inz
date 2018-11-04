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

	//global windows
	namedWindow("src", WINDOW_NORMAL);
	namedWindow("dst", WINDOW_NORMAL);

	//get maze
	draw_maze();

	//keep track of tank
	processImages("data\\img0.jpg");
	waitKey(0);
	
	return 0;
}