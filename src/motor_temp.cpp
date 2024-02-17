#include "main.hpp"
#include <string>
#include <algorithm>
#include <mutex>

namespace // functions/variables local to this file
{
int line_num = 1;
void print_temp(pros::Motor &motor, std::string name) {
  double temp = motor.get_temperature();
  if (temp >= 70) {
    pros::screen::set_pen(COLOR_PURPLE);
  } else if (temp >= 65) {
    pros::screen::set_pen(COLOR_RED);
  } else if (temp >= 60) {
    pros::screen::set_pen(COLOR_ORANGE);
  } else if (temp >= 55) {
    pros::screen::set_pen(COLOR_YELLOW);
  } else if (temp >= 45) {
    pros::screen::set_pen(COLOR_GREEN);
  } else {
    pros::screen::set_pen(COLOR_BLUE);
  }
  std::string text = name + ": " + std::to_string(temp);

  if(temp >= 55) {
    //add overheat warning to list for controller to display
    std::lock_guard<pros::Mutex> lock(hot_motor_list_lock);
    auto list_start = hot_motor_list.begin();
    auto list_end = hot_motor_list.begin();
    //only add the warning if it is not already on the list
    if(std::find(list_start, list_end, text) != list_end) {
      hot_motor_list.push_back(text);
    }
  }

  text += " (Port: " + std::to_string(motor.get_port()) + ")";
  pros::screen::print(TEXT_MEDIUM, line_num, text.c_str());
  line_num++;
}
} // namespace

// task that prints motor temps to brain screen
void monitor_temp() {
  while (true) {
    pros::screen::set_pen(COLOR_WHITE);
    pros::screen::print(TEXT_MEDIUM, 1, "Motor temps: (C)");
    line_num = 2;

    pros::delay(1000);
  }
}