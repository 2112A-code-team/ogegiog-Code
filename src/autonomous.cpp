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
  ez::as::auton_selector.call_selected_auton(); 
}

/*
ANGLE VALUES FOR TURNING:
Turns are relative to STARTING angle of robot, NOT the current angle
    0/360 (start angle)
  90 [ ] 270
     180
*/

void far_side_auton_awp() {
  const int DRIVE_SPEED = 110; 
  const int TURN_SPEED  = 90;
  const int SWING_SPEED = 90;
  double dist_from_wall = 10; // add distance sensor here

  chassis.reset_drive_sensor();

  //Drive forward
  chassis.set_drive_pid(dist_from_wall, DRIVE_SPEED, true);
  chassis.wait_drive();
  //Point towards match load bar
  chassis.set_turn_pid(135, TURN_SPEED);
  chassis.wait_drive();
  //Drive to match load bar
  chassis.set_drive_pid(dist_from_wall * 1.4 - 17, DRIVE_SPEED, true);
  chassis.wait_drive();
  //Intake triball
  //do_something();
  //Back away from match load bar and then turn
  chassis.set_drive_pid(-5, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(225, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(5, DRIVE_SPEED);
  chassis.wait_drive();
  //Face elevation bar backwards
  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();
  //Drive to elevation bar backwards
  chassis.set_drive_pid(-50, DRIVE_SPEED); // needs tuning
  chassis.wait_drive();
}

void near_side_auton_awp() {
  const int DRIVE_SPEED = 110; 
  const int TURN_SPEED  = 90;
  const int SWING_SPEED = 90;

  chassis.reset_drive_sensor();

  //Deploy intake
  //launch_position();
  //Drive and face to goal
  chassis.set_drive_pid(60, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(270, TURN_SPEED);
  chassis.wait_drive();
  //Allow distance from goal to vary without affecting auton
  chassis.set_tank(127,127);
  pros::delay(2000);
  //back away from goal and turn
  chassis.set_drive_pid(-6, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(180, TURN_SPEED);
  chassis.wait_drive();
  //Drive backwards to line up with elevation bar
  chassis.set_drive_pid(-60, DRIVE_SPEED, true);
  chassis.wait_drive();
  //Turn to face bar and drive to it
  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-60, DRIVE_SPEED, true); // needs tuning
  chassis.wait_drive();
}