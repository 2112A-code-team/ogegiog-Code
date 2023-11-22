#include <cmath>
#include "main.hpp"
#include <cassert>
#include <utility>
#include <tuple>

namespace // functions/variables local to this file
{
    double curve_joystick(double joy_val, double curve) {
        double abs_val = std::abs(joy_val) / 127;
        return joy_val * std::pow(abs_val, curve);
    }
    std::pair<double,double> get_joysticks(
    pros::controller_analog_e_t left_joystick, pros::controller_analog_e_t right_joystick,
    double left_curve = 1, double right_curve = 1, double deadzone = 5) {
      double left = master.get_analog(left_joystick);
      double right = master.get_analog(right_joystick);
      if(std::abs(left) >= deadzone) {
        left = curve_joystick(left, left_curve);
      } else {
        left = 0;
      }
      if(std::abs(right) >= deadzone) {
        right = curve_joystick(right, right_curve);
      } else {
        right  = 0;
      }
      if(controls::slow_joysticks()) {
        left *= 0.5;
        right *= 0.5;
      }
      if(controls::reverse_joysticks()) {
        left *= -1;
        right *= -1;
      }
      return std::pair(left, right);
    }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  pros::Task cata_task(cata_control);
  while (true) {
    // control intake
    if (controls::intake_forward() && controls::intake_backward()) {}
    else if (controls::intake_forward()) {intake_motor.move_velocity(200);}
    else if (controls::intake_backward()) {intake_motor.move_velocity(-200);}
    else {intake_motor.brake();}
    
    if(is_arcade) {
      // joysticks - arcade drive
      double left, right;
      if(controls::rapid_fire()) {
        // push all control to one joystick when rapid firing
        std::tie(left, right) = get_joysticks(ANALOG_LEFT_Y, ANALOG_LEFT_X);
      } else {
        std::tie(left, right) = get_joysticks(ANALOG_LEFT_Y, ANALOG_RIGHT_X);
      }
      // prevent joystick clipping:
      // see vexforum.com/t/arcade-and-x-arcade-drive-using-full-range-of-joystick-without-clipping/72614/2
      //double forward_speed = left - (left * std::abs(right))/(127*2);
      //double turn_speed = left - (right * std::abs(left))/(127*2);
      left_wheels.move(left + right);
      right_wheels.move(left - right);
    } else {
      //joysticks - tank drive
      double left, right;
      std::tie(left, right) = get_joysticks(ANALOG_LEFT_Y, ANALOG_RIGHT_Y);
      left_wheels.move(left);
      right_wheels.move(right);     
    }
    pros::delay(10);
  }
}