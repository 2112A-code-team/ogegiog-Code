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
    //control intake
    if (controls::intake_forward() && controls::intake_backward()) {}
    else if (controls::intake_forward()) {intake_motor.move_velocity(200);}
    else if (controls::intake_backward()) {intake_motor.move_velocity(-200);}
    else {intake_motor.brake();}
    
    //joysticks - tank drive
    const int deadzone = 5;
    double curved_left = curve_joystick(master.get_analog(ANALOG_LEFT_Y), 1);
    double curved_right = curve_joystick(master.get_analog(ANALOG_RIGHT_Y), 1);
    if(master.get_analog(ANALOG_LEFT_Y) >= deadzone) {
      left_wheels.move(curved_left);
    }
    if(master.get_analog(ANALOG_RIGHT_Y) >= deadzone) {
      right_wheels.move(curved_right);
    }
    pros::delay(10);
  }
}