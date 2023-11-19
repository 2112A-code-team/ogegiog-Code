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
        print_temp(left_wheels[0], "drive left front");
        print_temp(left_wheels[1], "drive left front");
        print_temp(right_wheels[0], "drive right front");
        print_temp(right_wheels[1], "drive right front");
        print_temp(cata_motor[0], "cata left");
        print_temp(cata_motor[1], "cata right");
        print_temp(intake_motor, "intake");
        pros::delay(1000);
    }
}