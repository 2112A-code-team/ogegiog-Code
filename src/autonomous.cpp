#include "main.hpp"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  pros::Task print_motor_task(monitor_temp); 
}

/*
ANGLE VALUES FOR TURNING:
Turns are relative to STARTING angle of robot, NOT the current angle
    0/360 (start angle)
  90 [ ] 270
     180
*/

void far_side_auton_awp() {

}

void near_side_auton_awp() {

}