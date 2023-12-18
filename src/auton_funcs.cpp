#include "main.hpp"

namespace // functions/variables local to this file
{
const double pi = 3.14159265358979323846;
const double gear_ratio = 84.0 / 12.0;
const double wheel_radius = 3.25;
const double wheel_circumference = wheel_radius * 2 * pi;
double deg_to_rad(double degrees) { return degrees * pi / 180; }
double rad_to_deg(double radians) { return radians * 180 / pi; }

double drive_kP = 0;
double drive_kD = 0;
double drive_kI = 0;

double turn_kP = 0;
double turn_kD = 0;
double turn_kI = 0;

double robot_x = 0;
double robot_y = 0;

} // namespace

void setPosition(double x, double y) {
  robot_x = x;
  robot_y = y;
}

double getPositionX() { return robot_x; }

double getPositionY() { return robot_y; }

void odometry(void *ignore) {
  double prev_left_encoder = 0;
  double prev_right_encoder = 0;
  left_front.tare_position();
  left_back.tare_position();
  right_front.tare_position();
  right_back.tare_position();
  while (true) {
    double left_encoder =
        (left_front.get_position() + left_back.get_position()) / 2;
    double right_encoder =
        (right_front.get_position() + right_back.get_position()) / 2;
    double left_delta = left_encoder - prev_left_encoder;
    double right_delta = right_encoder - prev_right_encoder;
    double dist =
        deg_to_rad((left_delta + right_delta) / 2) * wheel_radius / gear_ratio;
    double heading = deg_to_rad(360 - inertial.get_heading());
    double delta_x = cos(heading) * dist;
    double delta_y = sin(heading) * dist;
    prev_left_encoder = left_encoder;
    prev_right_encoder = right_encoder;
    robot_x += delta_x;
    robot_y += delta_y;
    pros::delay(20);
  }
}

void driveTo(double x, double y) {
  double last_dist_err = 0;
  double dist_err_accumulator = 0;
  double last_angle_err = 0;
  double angle_err_accumulator = 0;
  while (true) {
    double dist_err = std::hypot(x - robot_x, y - robot_y);
    double angle_err = rad_to_deg(std::atan2(x - robot_x, y - robot_y)) - (360 - inertial.get_heading());
    if(angle_err > 180) {angle_err -= 360;}
    if(angle_err < -180) {angle_err += 360;}
    dist_err_accumulator += dist_err;
    if(std::abs(dist_err) > 10) {dist_err_accumulator = 0;}
    angle_err_accumulator += angle_err;
    if(std::abs(angle_err) > 15) {angle_err_accumulator = 0;}
    double drive_power = drive_kP * dist_err + drive_kD * (dist_err - last_dist_err) + drive_kI * dist_err_accumulator;
    double turn_power = turn_kP * angle_err + turn_kD * (angle_err - last_angle_err) + turn_kI * angle_err_accumulator;
    left_wheels.move(drive_power - turn_power);
    right_wheels.move(drive_power + turn_power);
    last_dist_err = dist_err;
    last_angle_err = angle_err;
    if(std::abs(dist_err) < 1.5) break;
    pros::delay(20);
  }
}

void pointTowards(double x, double y) {
  pointTo(std::atan2(x,y));
}

void pointTo(double angle) {
  double last_angle_err = 0;
  double angle_err_accumulator = 0;
  while (true) {
    double angle_err = angle - (360 - inertial.get_heading());
    if(angle_err > 180) {angle_err -= 360;}
    if(angle_err < -180) {angle_err += 360;}
    angle_err_accumulator += angle_err;
    if(std::abs(angle_err) > 15) {angle_err_accumulator = 0;}
    double turn_power = turn_kP * angle_err + turn_kD * (angle_err - last_angle_err) + turn_kI * angle_err_accumulator;
    left_wheels.move(- turn_power);
    right_wheels.move(turn_power);
    last_angle_err = angle_err;
    if(std::abs(angle_err) < 1.5) break;
    pros::delay(20);
  }
}