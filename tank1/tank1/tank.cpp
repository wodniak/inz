#include "tank.h"
#include <iostream>

/* Steering routine 
      LEFT wheel: 
		0x41 A - full speed forward
		0x5A Z - medium speed forward
		0x53 S - full speed backwards
		0x58 X - medium speed backwards

	RIGHT wheel:
		0x46 F - full speed forward
		0x56 V - medium speed forward
		0x47 G - full speed backwards
		0x42 B - medium speed backwards   
		
		NO KEY PRESSED -> engine stop
		Check Virtual-Key Codes for more 
*/
void Tank::steer()
{
	/*LEFT wheel*/
	if (GetKeyState(0x41) & 0x800)		//key 'A'
	{
		left_wheel = 127;
	}
	else if (GetKeyState(0x5A) & 0x800)	//key 'Z'
	{
		left_wheel = 100;	//~80% power
	}
	else if (GetKeyState(0x53) & 0x800)	//key 'S'
	{
		left_wheel = 128;
	}
	else if (GetKeyState(0x58) & 0x800)	//key 'X'
	{
		left_wheel = 155;	//~80% power	
	}
	else
		left_wheel = 0;					//NO key


	/*RIGHT wheel*/
	if (GetKeyState(0x46) & 0x800)		//key 'F'
	{
		right_wheel = 127;
	}
	else if (GetKeyState(0x56) & 0x800)	//key 'V'
	{
		right_wheel = 100;	//~80% power
	}
	else if (GetKeyState(0x47) & 0x800)	//key 'G'
	{
		right_wheel = 128;
	}
	else if (GetKeyState(0x42) & 0x800)	//key 'B'
	{
		right_wheel = 155;	//~80% power
	}
	else
		right_wheel = 0;				//NO key

	/*Debug info*/
	std::cout << "L: " << int(left_wheel) << " R: " << int(right_wheel) << std::endl;
	
	/*Send both left and right steering value*/
	writeSerialPort(&left_wheel, sizeof(unsigned char));
	writeSerialPort(&right_wheel, sizeof(unsigned char));
}

/*Receiving routine*/
void Tank::read_sensors()
{
	char incomingData[MAX_DATA_LENGTH];

	/*Check number of bytes received  */
	int bytes_received = readSerialPort(incomingData, MAX_DATA_LENGTH);

	if (bytes_received > 0)
	{
		/*prints out data */
		std::cout << incomingData << std::endl;
	}
}