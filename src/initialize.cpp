#include "display/lv_core/lv_obj.h"
#include "main.hpp"
#include <fstream>




/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  
    flywheel_arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    left_wheels.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
    right_wheels.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
    lift.tare_position();

    all_tabs = lv_tabview_create(lv_scr_act(), nullptr);
    motor_temp_page = lv_tabview_add_tab(all_tabs, "Motors");
    //auton_select_page = lv_tabview_add_tab(all_tabs, "Auton");
    lemlib_debug_page = lv_tabview_add_tab(all_tabs, "Debug");

    //set_up_auton_selector();
    chassis.calibrate();

    pros::Task screen_task(monitor_temp);


    pros::Task lemlib_debug_task([=]{
        lv_obj_t* x_label = lv_label_create(lemlib_debug_page, nullptr);
        lv_obj_align(x_label, nullptr, LV_ALIGN_IN_TOP_LEFT, 25, 30);
        lv_obj_t* y_label = lv_label_create(lemlib_debug_page, nullptr);
        lv_obj_align(y_label, nullptr, LV_ALIGN_IN_TOP_LEFT, 25, 50);
        lv_obj_t* theta_label = lv_label_create(lemlib_debug_page, nullptr);
        lv_obj_align(theta_label, nullptr, LV_ALIGN_IN_TOP_LEFT, 25, 70);
        while(true) {
            lv_label_set_text(x_label, std::to_string(chassis.getPose().x).c_str());
            lv_label_set_text(y_label, std::to_string(chassis.getPose().y).c_str());
            lv_label_set_text(theta_label, std::to_string(chassis.getPose().theta).c_str());
            pros::delay(100);
        }
    });
}


/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  while (true) {
    pros::delay(100);
    int value = selector_pot.get_value();
    int range = 4095 / auton_list2.size();
    int selected_auton_index = std::min(value / range, static_cast<int>(auton_list2.size()) - 1);
    master.set_text(0, 0, auton_list2[selected_auton_index].first.c_str());
  }
}
