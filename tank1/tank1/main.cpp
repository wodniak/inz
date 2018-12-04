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
	Tank tank(port_name);
	if (tank.isConnected())
		cout << "Connection Established" << endl;
	else
		cout << "ERROR, check port name";
	/*
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
	frame = imread("data\\test.jpg", 1); 

	Myimgproc::init();

	//get maze
	Myimgproc::draw_maze(frame);
	Maze * maze = Myimgproc::create_graph2();
	maze->use_dikstra(); 
	maze->draw_solution(frame);

	int frame_number = 0;
	Point2i tank_position;
	double dist_line;
	//loop until end of video
	while (!frame.empty())
	{
		if (frame_number % 10 == 0)
		{
			tank_position = Myimgproc::processImages(frame);			//keep track of tank
			dist_line = maze->calc_dist_to_line(tank_position);			//calc distance to line


		}

		frame_number++;
		cap >> frame;

		//wait between frames
		waitKey(100);

	}
	
	return 0;
}
/*
																				
	Cart : (-1980 595) (2017 456)            Polar : (526.000000 1.535890)	
	Cart : (-1939 939) (2051 660)            Polar : (801.000000 1.500983)
	Cart : (-1997 - 106) (1997 104)           Polar : (-1.000000 1.623156)
	Cart : (645 - 2127) (1271 1824)           Polar : (-970.000000 2.984513)
	Cart : (-1935 994) (2055 715)            Polar : (857.000000 1.500983)
	Cart : (656 - 2184) (1488 1728)           Polar : (-1096.000000 2.932153)
	Cart : (652 - 2111) (1208 1850)           Polar : (-939.000000 3.001966)
	Cart : (665 - 2167) (1428 1760)           Polar : (-1066.000000 2.949606)
	Cart : (609 - 2174) (1441 1738)           Polar : (-1048.000000 2.932153)
	Cart : (602 - 2192) (1502 1706)           Polar : (-1080.000000 2.914700)
	Cart : (-1994 - 157) (1996 122)           Polar : (-18.000000 1.640610)
	Cart : (-1994 - 164) (2000 46)            Polar : (-59.000000 1.623156)
	Cart : (-1997 - 108) (2000 32)            Polar : (-38.000000 1.605703)
*/

