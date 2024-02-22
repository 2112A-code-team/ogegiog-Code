#include "main.hpp"
#include <fstream>
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
  std::cout << selected_auton;
  auto it = auton_list.find(selected_auton);
  if(it != auton_list.end()) {
    it->second.first();
  } else {
    std::cout << "NO AUTON FOUND";
  }
}

/*
ANGLE VALUES FOR TURNING:
Turns are relative to STARTING angle of robot, NOT the current angle
    0/360 (start angle)
  90 [ ] 270
     180
*/

static lv_obj_t* auton_desc;

lv_res_t change_auton_action(lv_obj_t * list_btn) {
  selected_auton = lv_list_get_btn_text(list_btn);
  if(pros::usd::is_installed()) {
        std::ofstream auton_file("usd/selected_auton");
        if(auton_file.good()) {
            auton_file << selected_auton;
        } else {
          std::cout << "something";
        }
  } else {
    std::cout << "something";
  }
  lv_label_set_text(auton_desc, (selected_auton + ": " + auton_list.find(selected_auton)->second.second).c_str());
  return LV_RES_OK;
}

void set_up_auton_selector() {

  if(pros::usd::is_installed()) {
        std::ifstream auton_file("usd/selected_auton");
        if(auton_file.good()) {
            auton_file >> selected_auton;
        } else {
          std::cout << "something";
        }
  } else {
    std::cout << "something";
  }

  //set up auton selector lvgl
  lv_obj_t* auton_select_list = lv_list_create(auton_select_page, nullptr);
  lv_obj_set_size(auton_select_list, 200, 200);
  lv_obj_align(auton_select_list, nullptr, LV_ALIGN_IN_TOP_LEFT, 20, 20);

  auton_desc = lv_label_create(auton_select_page, NULL);
  lv_obj_set_size(auton_desc, 200, 200);
  lv_obj_align(auton_desc, nullptr, LV_ALIGN_IN_TOP_LEFT, 240, 20);
  lv_label_set_long_mode(auton_desc, LV_LABEL_LONG_BREAK);

  for(auto i : auton_list) {
      lv_obj_t* list_el;
      list_el = lv_list_add(auton_select_list, nullptr, i.first.c_str(), &change_auton_action);
      if(i.first == selected_auton) {
        lv_list_focus(list_el, false);
      }
  }
}

void far_side_auton() {
  chassis.moveTo(0, 15, 1500);
  chassis.turnTo(10, 0, 1500);
  auto pose = chassis.getPose();
  std::cout << pose.x << std::endl;
  std::cout << pose.y << std::endl;
  std::cout << pose.theta << std::endl;
}

void near_side_auton() {

}

/*
void tune_pid() {

  int now = pros::millis();
  chassis.moveTo(0, 10, 10000, 9999);
  int duration = pros::millis() - now;
  int lastDuration = duration;
  auto const initalController = lateralController;
  auto lastGoodController = lateralController;

  chassis = lemlib::Chassis(drivetrain, initalController, angularController, sensors);
  chassis.moveTo(0, 0, 10000, 150);

  while (true) {
    {
      while (true) {
        int now = pros::millis();
        chassis = lemlib::Chassis(drivetrain, lateralController, angularController, sensors);
        pros::delay(100);
        const double home_dist = front_distance.get()/25.4;
        chassis.moveTo(0, 10, 10000, 9999);
        int duration = pros::millis() - now;
        pros::delay(100);
        double dist = front_distance.get()/25.4 - home_dist;

        chassis = lemlib::Chassis(drivetrain, initalController, angularController, sensors);
        chassis.moveTo(0, 0, 10000, 150);

        if(duration < 1500 && dist < 11.5) {
          lateralController.kP *= 2;
        } else {
          break;
        }
        
    }
    }
    {
      int lastDuration = 11000;
      while (true) {
        int now = pros::millis();
        chassis = lemlib::Chassis(drivetrain, lateralController, angularController, sensors);
        pros::delay(100);
        const double home_dist = front_distance.get()/25.4;
        chassis.moveTo(0, 10, 10000, 9999);
        int duration = pros::millis() - now;
        pros::delay(100);
        double dist = front_distance.get()/25.4 - home_dist;

        chassis = lemlib::Chassis(drivetrain, initalController, angularController, sensors);
        chassis.moveTo(0, 0, 10000, 150);

        if(duration <= lastDuration && dist > 9.5) {
          lateralController.kD += 10;
        } else {
          lateralController.kP -= 10;
          break;
        }
        lastDuration = duration;
      }
    }
    int now = pros::millis();
    chassis = lemlib::Chassis(drivetrain, lateralController, angularController, sensors);
    chassis.moveTo(0, 10, 10000, 9999);
    int duration = pros::millis() - now;
    
    chassis = lemlib::Chassis(drivetrain, initalController, angularController, sensors);
    chassis.moveTo(0, 0, 10000, 150);

    if(duration < lastDuration) {
      lastGoodController = lateralController;
    } else {
      break;
    }
  }
  lateralController = lastGoodController;
  std::cout << "kp:" << lateralController.kP << std::endl;
  std::cout << "kd:" << lateralController.kD << std::endl;
  chassis = lemlib::Chassis(drivetrain, lateralController, angularController, sensors);
  chassis.moveTo(0, 10, 1000);
  //chassis.moveTo(0, 0, 10000, 50);

}
*/

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
    master.set_text(0, 0, text);
    pros::delay(100);
  }
}

void skills() {

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
        master.set_text(0,0, std::to_string(static_cast<int>(lowerRangeController.kP)) + " " + std::to_string(static_cast<int>(lowerRangeController.kD)));
        return;
      }
    }
    lowerRangeController = upperRangeController;
    upperRangeController.kP *= kp_increase_factor;
  }
}