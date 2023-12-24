#include "main.hpp"

namespace // functions/variables local to this file
{
const int launch_angle = 66;
const int block_angle = 44;
int position = 0;
//enum {NEED_RESET, LAUNCH, LOCK, HOME} next_position = NEED_RESET;

void launch_position() {
  position = 0;
  //next_position = NEED_RESET;
  cata_motor.move_velocity(40);
  while (cata_sensor.get_angle() <= launch_angle ||
         cata_sensor.get_angle() >= 340) {
    pros::delay(10);
  }
  cata_motor.move_velocity(0);
  position = 3;
  //next_position = HOME;
}
void lock_position() {
  position = 0;
  cata_motor.move_velocity(40);
  while (cata_sensor.get_angle() <= block_angle ||
         cata_sensor.get_angle() >= 340) {
    pros::delay(10);
  }
  cata_motor.move_velocity(0);
  position = 2;
}
void home_position() {
  position = 0;
  cata_motor.move_velocity(40);
  while (cata_sensor.get_angle() >= 10) {
    pros::delay(10);
  }
  cata_motor.move_velocity(0);
  cata_motor.move_relative(180, 40);
  while (!((cata_motor.get_positions()[0] < 185) &&
           (cata_motor.get_positions()[0] > 175))) {
    pros::delay(10);
  }
  position = 1;
}
} // namespace

void cata_control(void *ignore) {
  while (true) {
    while (position == 0) {
      launch_position();
    }
    while (position == 1) {
      if (controls::launch() || controls::rapid_fire()) {
        launch_position();
      } else if (controls::lock_intake()) {
        lock_position();
      }
    }
    while (position == 2) {
      if (controls::lock_intake()) {
        launch_position();
      }
    }
    while (position == 3) {
      if (controls::launch() || controls::rapid_fire()) {
        home_position();
      }
    }
  }
}
