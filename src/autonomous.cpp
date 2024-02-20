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
        }
  }
  lv_label_set_text(auton_desc, (selected_auton + ": " + auton_list[selected_auton].second).c_str());
  return LV_RES_OK;
}

void set_up_auton_selector() {
  if(pros::usd::is_installed()) {
        std::ifstream auton_file("usd/selected_auton");
        if(auton_file.good()) {
            auton_file >> selected_auton;
        }
  }

  //set up auton selector
  lv_obj_t* auton_select_list = lv_list_create(auton_select_page, nullptr);
  lv_obj_set_size(auton_select_list, 200, 200);
  lv_obj_align(auton_select_list, nullptr, LV_ALIGN_IN_TOP_LEFT, 20, 20);

  auton_desc = lv_label_create(auton_select_page, NULL);
  lv_obj_set_size(auton_desc, 200, 200);
  lv_obj_align(auton_desc, nullptr, LV_ALIGN_IN_TOP_LEFT, 240, 20);
  lv_label_set_long_mode(auton_desc, LV_LABEL_LONG_BREAK);

  for(auto i : auton_list) {
      lv_list_add(auton_select_list, nullptr, i.first.c_str(), &change_auton_action);
  }
}

void far_side_auton() {

}

void near_side_auton() {

}

void tune_pid() {
  chassis.moveTo(10, 0 , 10);
}

void skills() {

}