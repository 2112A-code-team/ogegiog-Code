#include "autonomous.hpp"
#include "globals.hpp"
#include "main.hpp"
#include <iostream>

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
/*  std::cout << selected_auton;
  auto it = auton_list.find(selected_auton);
  if(it != auton_list.end()) {
    it->second.first();
  } else {
    std::cout << "NO AUTON FOUND";
  } */

  
  int value = selector_pot.get_value();
  int range = 4095 / auton_list2.size();
  int selected_auton_index = std::min(value / range, static_cast<int>(auton_list2.size()) - 1);
  pros::delay(50);
  master.set_text(1, 1, auton_list2[selected_auton_index].first.c_str());
  pros::delay(50);
  (auton_list2[selected_auton_index].second)();
}


static void turn_and_move(double x, double y, int timeout) {
  chassis.moveTo(x, y, timeout);
  chassis.turnTo(x, y, timeout);
}

static void move_away_from_wall() {
  chassis.moveTo(chassis.getPose().x, chassis.getPose().y + 9, 1000);
  chassis.moveTo(chassis.getPose().x, chassis.getPose().y + 6, 1000);
}

void far_side_auton() {
  const double distFromWall = front_distance.get()/25.4 + 5;
  chassis.setPose(-180 + distFromWall, -173.25, 0);

  move_away_from_wall();

  //go to match load zone
  turn_and_move(-120, -120, 1000);
  turn_and_move(-137, -137, 1000);
  flywheel_arm.move_absolute(500, 200);
  flywheel.move_velocity(-300);
  pros::delay(1000);
  chassis.moveTo(-120, -120, 1000);

  //drop triball
  chassis.turnTo(-60, -150, 750);
  flywheel.move_velocity(0);
  flywheel_arm.move_absolute(0, 200);
  pros::delay(500);

  //push triballs in neutral zone
  turn_and_move(-60, -60, 2500);
  turn_and_move(-90, -26, 1000);
  chassis.turnTo(180, -26, 1000);
  wings.set_value(1);
  left_wheels.move(127);
  right_wheels.move(127);
  pros::delay(1500);
  left_wheels.move(0);
  right_wheels.move(0);
  chassis.setPose(-8, -26, chassis.getPose().theta);
  chassis.moveTo(-35, -26, 500);
  turn_and_move(-120, -120, 2500);
  turn_and_move(-60, -150, 1000);
  chassis.moveTo(-10, -150, 1000);
  //master.set_text(1, 1, "far side end");
}

void near_side_auton() {
  //master.set_text(1, 1, "near side start");
  //const double distFromWall = front_distance.get()/25.4 + 5;
  //chassis.setPose(180 - distFromWall, -173.25, 180);
  chassis.setPose(60, -150, 270);
  
  //pick up triball under bar
  flywheel_arm.move_absolute(500, 200);
  flywheel.move_velocity(-300);
  pros::delay(1000);
  chassis.moveTo(9, -150, 1000);

  //move back
  chassis.moveTo(90, -150, 1000);

  //go to side of goal
  chassis.turnTo(60, -180, 1000);
  chassis.moveTo(150, -90, 1500);
  chassis.turnTo(150, -180, 1000);

  //ram goal
  chassis.moveTo(150, -67, 1000);
  chassis.moveTo(150, -90, 1500);
  
  //turn and ram again
  chassis.turnTo(150, 0, 1000);
  flywheel_arm.move_absolute(0, 200);
  flywheel.move_velocity(0);
  pros::delay(1000);
  chassis.moveTo(150, -67, 2000);
  chassis.moveTo(150, -90, 1500);

  //move between two triballs on auton line
  turn_and_move(90, -90, 2000);
  turn_and_move(33, 0, 2500);
  chassis.turnTo(-180, 0, 1000);

  //intake one triball
  flywheel_arm.move_absolute(500, 200);
  flywheel.move_velocity(-300);
  pros::delay(1000);

  chassis.turnTo(180, 0, 1500);
  flywheel_arm.move_absolute(0, 200);
  flywheel.move_velocity(0);
  pros::delay(1000);

  left_wheels.move(127);
  right_wheels.move(127);


/*
  move_away_from_wall();

  turn_and_move(150, -68, 1500);
  turn_and_move(120, -120, 1000);
  turn_and_move(33, 0, 2500);
  chassis.turnTo(-180, 0, 1000);
  flywheel_arm.move_absolute(500, 200);
  flywheel.move_velocity(-300);
  pros::delay(1000);
  left_wheels.move(-127);
  right_wheels.move(-127);
  chassis.setPose(113, 0, chassis.getPose().theta);
  chassis.moveTo(100, 0, 1000);
  chassis.turnTo(180, 0, 1000);
  flywheel.move_velocity(0);
  flywheel_arm.move_absolute(0, 200);
  left_wheels.move(127);
  right_wheels.move(127);
 // master.set_text(1, 1, "near side end");
*/
}

void skills() {
  //master.set_text(1, 1, "skills start");
  //const double distFromWall = front_distance.get()/25.4 + 5;
  chassis.setPose(-120, -130, 90);
  flywheel.move_velocity(300);
  pros::delay(30 * 1000);
  chassis.moveTo(60, -130, 5000);
  wings.set_value(1);
  turn_and_move(110, -110, 2500);
  chassis.turnTo(150, 0, 1500);
  left_wheels.move(127);
  right_wheels.move(127);
  pros::delay(4000);
  left_wheels.move(0);
  pros::delay(3000);
  right_wheels.move(0);

  //move_away_from_wall();

  //chassis.moveTo(0, 15, 1500);
  //chassis.turnTo(10, 0, 1500);
  //master.set_text(1, 1, "skills end");
}

void manual_tune_pid() {
  master.clear();
  pros::delay(100);
  while(true) {
    if(controls::fvw_up()) {
      chassis = lemlib::Chassis(drivetrain, lateralController, angularController, sensors);
      chassis.moveTo(0, 10, 3000);
    }
    if(controls::fvw_down()) {
      chassis = lemlib::Chassis(drivetrain, lateralController, angularController, sensors);
      chassis.moveTo(0, 0, 3000);
    }
    if(controls::fly_arm_forward()) {
      lateralController.kP -= 10;
      pros::delay(500);
    }
    if(controls::fly_arm_backward()) {
      lateralController.kP += 10;
      pros::delay(500);
    }
    if(controls::lift_up()) {
      lateralController.kD += 10;
      pros::delay(500);
    }
    if(controls::lift_down()) {
      lateralController.kD -= 10;
      pros::delay(500);
    }
    std::string text = "kp: " + std::to_string(static_cast<int>(lateralController.kP));
    text += "kd: " + std::to_string(static_cast<int>(lateralController.kD));
    master.set_text(0, 0, text.c_str());
    pros::delay(100);
  }
}



static int time_chassis_move(lemlib::ChassisController_t newLateralController) {
  chassis = lemlib::Chassis(drivetrain, newLateralController, angularController, sensors);
  int lastTime = pros::millis();
  chassis.moveTo(0, 10, 5500);
  int duration = pros::millis() - lastTime;
  chassis.moveTo(0, 0, 5500);
  return duration;
}

void tune_pid() {
  const double kp_increase_factor = 1.2;
  lateralController.largeErrorTimeout *= 100;
  lateralController.smallErrorTimeout *= 100;
  auto lowerRangeController = lateralController;
  auto upperRangeController = lateralController;
  upperRangeController.kP *= kp_increase_factor;
  while (true) {
    int duration = time_chassis_move(upperRangeController);
    if(duration > 4500) {
      // the drivetrain oscillated
      int i = 0;
      while (true) {
        // keep increasing kD till oscillation stops
        lowerRangeController.kD += 10;
        int duration = time_chassis_move(lowerRangeController);
        if( duration < 1200) {
          //we stopped oscillation
          break;
        }
        i++;
        if( i == 20) { 
          //oscillation won't stop
          goto too_high_kp;
        }
      }
      too_high_kp: {
        pros::delay(100);
        std::cout << lowerRangeController.kP << std::endl;
        pros::delay(100);
        std::cout << lowerRangeController.kD << std::endl;
        pros::delay(100);
        master.clear_line(0);
        pros::delay(100);
        std::string text = std::to_string(static_cast<int>(lowerRangeController.kP)) + " " + std::to_string(static_cast<int>(lowerRangeController.kD));
        master.set_text(0,0, text.c_str());
        return;
      }
    }
    lowerRangeController = upperRangeController;
    upperRangeController.kP *= kp_increase_factor;
  }
}