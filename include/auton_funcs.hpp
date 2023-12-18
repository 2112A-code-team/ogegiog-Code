#ifndef _AUTON_FUNCS_HPP_
#define _AUTON_FUNCS_HPP_

#include "main.hpp"

void odometry(void*);
void driveTo(double x, double y);
void pointTowards(double x, double y);
void pointTo(double angle);
void setPosition(double x, double y);
double getPositionX();
double getPositionY();

#endif