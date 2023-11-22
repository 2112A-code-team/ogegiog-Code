#include "main.hpp"
namespace  // functions/variables local to this file
{
    int line_num = 1;
    void print_temp(pros::Motor &motor, std::string name) {
        std::string text = name + ": " + std::to_string(motor.get_temperature());
        pros::screen::print(pros::E_TEXT_SMALL, line_num, text.c_str());
        line_num++;
    }  
}

//task that prints motor temps to brain screen
void monitor_temp(void* ignore) {
    while (true) {
        pros::screen::print(pros::E_TEXT_SMALL, 1, "Motor temps: (C)");
        line_num = 2;
        print_temp(left_front, "drive left front");
        print_temp(left_back, "drive left back");
        print_temp(right_front, "drive right front");
        print_temp(right_back, "drive right back");
        print_temp(cata_left, "cata left");
        print_temp(cata_right, "cata right");
        print_temp(intake_motor, "intake");
        pros::delay(1000);
    }
}