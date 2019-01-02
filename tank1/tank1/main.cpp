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

	Myimgproc::createAllWindows();			//open all opencv windows 
	//VideoCapture cap(0);					//Create a VideoCapture object and open the input file
	VideoCapture cap(0);	//mock video

	if (!cap.isOpened())					// Check if camera opened successfully 
	{
		cout << "Error opening video stream or file" << endl;
		system("pause");
		return 15;

	}	
	//TODO gives unknown error - investigate
	/*
	cap.set(CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(CAP_PROP_FRAME_HEIGHT, 720);
	if (cap.get(CAP_PROP_FRAME_HEIGHT) != 720 || cap.get(CAP_PROP_FRAME_WIDTH) != 1280)
		cout << "unable to set 1280*720\n";
	*/


	Mat frame;				//matrix for each frame in video
	cap >> frame;			//get first frame without robot
	frame = imread("data\\mock3.jpg", 1); //first frame - mock

	Myimgproc::init();
	Myimgproc::draw_maze(frame);		//get maze

	
	//create maze and calculate shortest path
	Maze * maze = Myimgproc::create_graph2();
	maze->use_dikstra(); 
	maze->draw_solution(frame);


	int frame_number = 0;
	Point2i tank_position;
	int angle;
	double dist_to_line, cross_track_error;
	bool tank_turn_right;

	//loop until end of video
	while (tank.isConnected())
	{

		//tank.steer();
		//Sleep(100);

		if (frame_number % 3 == 0)			//every nth frame update steering
		{
			/*
				STEERING ROUTINE
			*/
			tie(tank_position,angle) = Myimgproc::processImages(frame);			//keep track of tank center & angle
			tie(dist_to_line, cross_track_error, tank_turn_right) = maze->calc_dist_to_line(tank_position,angle);		//calc distance to node
			
			tank.steer_auto(cross_track_error, tank_position, tank_turn_right);		//send steering values to robot
			
			maze->draw_line_to_node(frame, tank_position);		//just draw line
			maze->draw_solution(frame);

		}

		//get next frame
		frame_number++;
		cap >> frame;

		//wait between frames
		waitKey(100);
	}
	return 0;
	system("pause");

}
