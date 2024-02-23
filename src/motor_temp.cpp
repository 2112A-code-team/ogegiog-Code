#include "main.hpp"
#include <string>
#include <algorithm>
#include <mutex>
#include <array>




int line_num = 1;
void print_temp(pros::Motor &motor, std::string name, lv_obj_t* label) {
  int temp = static_cast<int>(motor.get_temperature());
  std::string text = name + ": " + std::to_string(temp) + "C";
  if(temp >= 55) {
    Alert temp_alert = Alert(text);
    controller_alerts.addUniqueAlert(temp_alert);
  }
  if (temp >= 70) {
    text = "#ff00ff " + text;
  } else if (temp >= 65) {
    text = "#ff0000 " + text;
  } else if (temp >= 60) {
    text = "#ffa500 " + text;
  } else if (temp >= 55) {
    text = "#ffff00 " + text;
  } else if (temp >= 45) {
    text = "#00ff00 " + text;
  } else {
    text = "#00ffff " + text;
  }
  text += " (Port " + std::to_string(motor.get_port()) + ")#";

  lv_label_set_text(label, text.c_str());
}


// task that prints motor temps to brain screen
void monitor_temp() {
  std::array<lv_obj_t*, 8> labels;
  for(int i = 0; i < 8; i++) {
    labels[i] = lv_label_create(motor_temp_page, nullptr);
    lv_obj_align(labels[i], nullptr, LV_ALIGN_IN_TOP_LEFT, 10, 20 * i + 20);
    lv_label_set_recolor(labels[i], true);
  }
  while (true) {
    print_temp(left_front, "drive lf", labels[0]);
    print_temp(left_back, "drive lb", labels[1]);
    print_temp(right_front, "drive rf", labels[2]);
    print_temp(right_back, "drive rb", labels[3]);
    print_temp(flywheel_arm, "fly arm", labels[4]);
    print_temp(flywheel, "flywheel", labels[5]);
    print_temp(lift_left, "lift l", labels[6]);
    print_temp(lift_right, "lift r", labels[7]);
    pros::delay(1000);
  }
}