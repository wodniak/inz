#include "PID.h"

/*	@!brief : 
 *	@param cross_track_error : 
 *	@param tank_position	:	
 *	@return : error after PID 
 */
double PID::calculate_PID(double & cross_track_error)
{
	// Proportional term
	double Pout = kp * cross_track_error;

	// Integral term
	integral += cross_track_error * dt;
	double Iout = ki * integral; 

	// Derivative term
	double derivative = (cross_track_error - pre_error) / dt;
	double Dout = kd * derivative;

	// Calculate total output
	double output = Pout + Iout + Dout;

	// Restrict to max/min
	if (output > _max)
		output = _max;
	else if (output < _min)
		output = _min;
	
	// Save error to previous error
	pre_error = cross_track_error;

	return output;
}