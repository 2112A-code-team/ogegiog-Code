#include <cmath>
#include "main.hpp"
#include <cassert>

namespace //these functions are local to this file
{
    // Curves the joystick values to provide greater control
    // \param joy_val The value of the joystick: [-127, 127]
    // \param curve Controls the intensity of curve, should not be negative
    // \return Curved joystick value: [-127, 127]
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
      double curved_left = 0;
      double curved_right = 0;
      if(master.get_analog(ANALOG_LEFT_Y) >= deadzone) {
        curved_left = curve_joystick(master.get_analog(ANALOG_LEFT_Y), 1);
      }
      if(master.get_analog(ANALOG_RIGHT_Y) >= deadzone) {
        curved_right = curve_joystick(master.get_analog(ANALOG_RIGHT_Y), 1);
      }
      if(controls::slow_joystick()) {
        curved_left *= 0.5;
        curved_right *= 0.5;
      }
      // prevent joystick clipping:
      // see vexforum.com/t/arcade-and-x-arcade-drive-using-full-range-of-joystick-without-clipping/72614/2
      double forward_speed = curved_left - (curved_left * std::abs(curved_right))/(127*2);
      double turn_speed = curved_left - (curved_right * std::abs(curved_left))/(127*2);
      if(controls::toggle_drive_direction()) {
        forward_speed *= -1;
        turn_speed *= -1;
      }
      left_wheels.move(forward_speed + turn_speed);
      right_wheels.move(forward_speed+ turn_speed);
    } else {
      //joysticks - tank drive
      const int deadzone = 5;
      double curved_left = 0;
      double curved_right = 0;
      if(master.get_analog(ANALOG_LEFT_Y) >= deadzone) {
        curved_left = curve_joystick(master.get_analog(ANALOG_LEFT_Y), 1);
      }
      if(master.get_analog(ANALOG_RIGHT_Y) >= deadzone) {
        curved_right = curve_joystick(master.get_analog(ANALOG_RIGHT_Y), 1);
      }
      if(controls::slow_joystick()) {
        curved_left *= 0.5;
        curved_right *= 0.5;
      }
      if(controls::toggle_drive_direction()) {
        curved_left *= -1;
        curved_right *= -1;
      }
      left_wheels.move(curved_left);
      right_wheels.move(curved_right);
      
    }
    pros::delay(10);
  }
}