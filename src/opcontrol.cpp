#include <cmath>
#include "main.hpp"
#include <cassert>

namespace // functions/variables local to this file
{
    double curve_joystick(double joy_val, double curve) {
        double abs_val = std::abs(joy_val) / 127;
        return joy_val * std::pow(abs_val, curve);
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
      const int deadzone = 5;
      double left_joystick = master.get_analog(ANALOG_LEFT_Y);
      double right_joystick = master.get_analog(ANALOG_RIGHT_X);
      if(controls::toggle_rapid_fire()) {
        // push all control to one joystick when rapid firing
        right_joystick = master.get_analog(ANALOG_LEFT_X);
      }
      if(std::abs(left_joystick) > deadzone) {
        left_joystick = curve_joystick(left_joystick, 1);
      } else {
        left_joystick = 0;
      }
      if(std::abs(right_joystick) < deadzone) {
        right_joystick = curve_joystick(right_joystick, 1);
      } else {
        right_joystick  = 0;
      }
      if(controls::slow_joysticks()) {
        left_joystick *= 0.5;
        right_joystick *= 0.5;
      }
      if(controls::reverse_joysticks()) {
        forward_speed *= -1;
        turn_speed *= -1;
      }
      // prevent joystick clipping:
      // see vexforum.com/t/arcade-and-x-arcade-drive-using-full-range-of-joystick-without-clipping/72614/2
      double forward_speed = left_joystick - (left_joystick * std::abs(right_joystick))/(127*2);
      double turn_speed = left_joystick - (right_joystick * std::abs(left_joystick))/(127*2);
      left_wheels.move(forward_speed + turn_speed);
      right_wheels.move(forward_speed+ turn_speed);
    } else {
      //joysticks - tank drive
      const int deadzone = 5;
      double left_joystick = 0;
      double right_joystick = 0;
      if(master.get_analog(ANALOG_LEFT_Y) >= deadzone) {
        left_joystick = curve_joystick(master.get_analog(ANALOG_LEFT_Y), 1);
      }
      if(master.get_analog(ANALOG_RIGHT_Y) >= deadzone) {
        right_joystick = curve_joystick(master.get_analog(ANALOG_RIGHT_Y), 1);
      }
      if(controls::slow_joysticks()) {
        left_joystick *= 0.5;
        right_joystick *= 0.5;
      }
      if(controls::reverse_joysticks()) {
        left_joystick *= -1;
        right_joystick *= -1;
      }
      left_wheels.move(left_joystick);
      right_wheels.move(right_joystick);     
    }
    pros::delay(10);
  }
}