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
	double min, max;
	double dt, pre_error, integral;
public:
	PID(float kp, float ki, float kd) :kp(kp), ki(ki), kd(kd) {};
	double & calculate_PID(double dist_to_line, double tank_position);

};


#endif // PID_H