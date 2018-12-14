#pragma once
#ifndef TANK_H
#define TANK_H
#include<opencv2/opencv.hpp>
#include "SerialPort.h"
#include "PID.h"
/*main tank interface*/
class Tank : public SerialPort
{
private:
	unsigned char  left_wheel;
	unsigned char  right_wheel;

	cv::Point2i position;

	//PID controller
	PID * pid_control;

public:
	/*only uses Serialport constructor*/
	Tank(char *portName) : SerialPort(portName) 
	{ 
		position = cv::Point2i(0,0);
		pid_control = new PID(3, 2, 1.5, 0.1);	//P	I	D	dT
	};
	
	/*	@!brief : 
	 *	@!param pid_output : value of calculated error with pid
	 *	@return : void
	 */
	void steer_auto(double & cross_track_error, double & tank_position);

	/*Steering routine*/
	void steer();

	/*Receiving routine*/
	void read_sensors();

	/*
	 * getter and setter for position
	 */
	void update_position(cv::Point2i p) { position = p; };
	cv::Point2i & get_position(void) { return position; };
};

#endif // TANK_H