#include "main.hpp"
#include <string>
#include <algorithm>
#include <mutex>
#include <array>




int line_num = 1;
void print_temp(pros::Motor &motor, std::string name, lv_obj_t* label) {
  /*
  static lv_style_t purple_style = lv_style_plain;
  purple_style.body.main_color = LV_COLOR_BLACK;
  purple_style.body.grad_color = LV_COLOR_BLACK;
  purple_style.text.font = &lv_font_dejavu_20;
  purple_style.text.letter_space = 2;
  purple_style.text.line_space = 1;
  static lv_style_t red_style = purple_style;
  static lv_style_t orange_style = purple_style;
  static lv_style_t yellow_style = purple_style;
  static lv_style_t green_style = purple_style;
  static lv_style_t blue_style = purple_style;

  purple_style.text.color = LV_COLOR_PURPLE;
  red_style.text.color = LV_COLOR_RED;
  orange_style.text.color = LV_COLOR_ORANGE;
  yellow_style.text.color = LV_COLOR_YELLOW;
  green_style.text.color = LV_COLOR_GREEN;
  blue_style.text.color = LV_COLOR_AQUA;
*/
  int temp = static_cast<int>(motor.get_temperature());
  std::string text;
  if (temp >= 70) {
    text += "#ff00ff ";
  } else if (temp >= 65) {
    text += "#ff0000 ";
  } else if (temp >= 60) {
    text += "#ffa500 ";
  } else if (temp >= 55) {
    text += "#ffff00 ";
  } else if (temp >= 45) {
    text += "#00ff00 ";
  } else {
    text += "#00ffff ";
  }
  text += name + ": " + std::to_string(temp) + "C";
  text += " (Port " + std::to_string(motor.get_port()) + ")#";

  if(temp >= 55) {
    Alert temp_alert = Alert(text);
    controller_alerts.addUniqueAlert(temp_alert);
  }

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