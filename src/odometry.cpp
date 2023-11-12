#include "main.hpp"

namespace
{
    const double pi = 3.14159265358979323846;
    const double gear_ratio = 84.0/12.0;
    const double wheel_radius = 3.25;
    const double wheel_circumference = wheel_radius * 2 * pi;
    double deg_to_rad(double degrees) {
        return degrees * pi / 180;
    }
} // functions/variables local to this file



void odometry(void* ignore) {
    double prev_left_encoder = (left_front.get_position() + left_back.get_position()) / 2;
    double prev_right_encoder = (right_front.get_position() + right_back.get_position()) / 2;
    while (true) {
        double left_encoder = (left_front.get_position() + left_back.get_position()) / 2;
        double right_encoder = (right_front.get_position() + right_back.get_position()) / 2;
        double left_delta = left_encoder - prev_left_encoder;
        double right_delta = right_encoder - prev_right_encoder;
        double dist = deg_to_rad((left_delta + right_delta)/2) * wheel_radius / gear_ratio;
        double heading = deg_to_rad(inertial.get_heading());
        double delta_x = cos(heading) * dist;
        double delta_y = sin(heading) * dist;
        prev_left_encoder = left_encoder;
        prev_right_encoder = right_encoder;
        robot_x += delta_x;
        robot_y += delta_y;
        pros::delay(20);
    }
}