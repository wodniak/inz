#pragma once
#ifndef TANK_H
#define TANK_H

#include "SerialPort.h"

/*main tank interface*/
class Tank : public SerialPort
{
private:
	unsigned char  left_wheel;
	unsigned char  right_wheel;

public:
	/*only uses Serialport constructor*/
	Tank(char *portName) : SerialPort(portName) {}
	
	/*Steering routine*/
	void steer();

	/*Receiving routine*/
	void read_sensors();
};

#endif // TANK_H