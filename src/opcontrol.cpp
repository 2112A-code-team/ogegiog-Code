#include "main.hpp"
#include <cmath>

namespace // functions/variables local to this file
{
double curve_joystick(double joy_val, double curve) {
  double abs_val = std::abs(joy_val) / 127;
  return joy_val * std::pow(abs_val, curve);
}
double get_joystick(pros::controller_analog_e_t joystick, double curve = 1,
                    double deadzone = 5) {
  double joystick_val = master.get_analog(joystick);
  if (std::abs(joystick_val) >= deadzone) {
    joystick_val = curve_joystick(joystick_val, curve);
  } else {
    joystick_val = 0;
  }
  if (controls::slow_joysticks()) {
    joystick_val *= 0.5;
  }
  if (controls::reverse_joysticks()) {
    joystick_val *= -1;
  }
  return joystick_val;
}
} // namespace

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
    if (controls::intake_forward() && controls::intake_backward()) {
      intake_motor.brake();
    } else if (controls::intake_forward()) {
      intake_motor.move_velocity(200);
    } else if (controls::intake_backward()) {
      intake_motor.move_velocity(-200);
    } else {
      intake_motor.brake();
    }
    // joysticks - arcade drive
    double forward_speed, turn_speed;
    forward_speed = get_joystick(ANALOG_LEFT_Y);
    if (controls::rapid_fire()) {
      // push all control to one joystick when rapid firing
      turn_speed = get_joystick(ANALOG_LEFT_X);
    } else {
      turn_speed = get_joystick(ANALOG_RIGHT_X);
    }
    // vexforum.com/t/arcade-and-x-arcade-drive-using-full-range-of-joystick-without-clipping/72614/2
    // double forward_speed = left - (left * std::abs(right))/(127*2);
    // double turn_speed = left - (right * std::abs(left))/(127*2);
    chassis.set_tank(forward_speed + turn_speed, forward_speed - turn_speed);
    pros::delay(10);
  }
}