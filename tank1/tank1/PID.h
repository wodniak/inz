#pragma once
#ifndef PID_H
#define PID_H

class PID
{
private:
	/*
	 * kp -  proportional gain
	 * ki -  Integral gain
	 * kd -  derivative gain
	 */
	double kp, ki, kd;
	double _min, _max;		//limits
	double dt, pre_error, integral;

public:
	PID(double kp, double ki, double kd, double dt) :
		kp(kp),	ki(ki), kd(kd), dt(dt),
		pre_error(0),integral(0),
		_min(-5000),_max(5000){};

	double calculate_PID(double & cross_track_error);

};


#endif // PID_H