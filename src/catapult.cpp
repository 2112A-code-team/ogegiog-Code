#include "main.hpp"

namespace  // functions/variables local to this file
{
  const int launch_angle = 66;
  const int block_angle = 44;
  int position = 0;

  void launch_position() {
    position = 0;
    cata_motor.move_velocity(40);
    while (cata_sensor.get_angle() <= launch_angle || cata_sensor.get_angle() >= 340) {
      pros::delay(10);
    }
    cata_motor.move_velocity(0);
    position = 3;
  }
  void home_position() {
    position = 0;
    cata_motor.move_velocity(40);
    while (cata_sensor.get_angle() <= block_angle || cata_sensor.get_angle() >= 340) {
      pros::delay(10);
    }
    cata_motor.move_velocity(0);
    position = 2;
  }
  void lock_position() {
    position = 0;
    cata_motor.move_velocity(40);
    while (cata_sensor.get_angle() >= 10) {
      pros::delay(10);
    }
    cata_motor.move_velocity(0);
    cata_motor.move_relative(180, 40);
    while (!((cata_motor.get_positions()[0] < 185) && (cata_motor.get_positions()[0] > 175))) {
      pros::delay(10);
    }
    position = 1;
  }
}


void cata_control(void* ignore) {
  bool is_rapid_fire = false;
  while (true) {
    if(controls::toggle_rapid_fire()) {
      is_rapid_fire = !is_rapid_fire;
    }
    while (position == 0) {
      launch_position();
    }
    while (position == 1) {
      if(controls::launch() || is_rapid_fire) {
        launch_position();
      } else if(controls::lock_intake()) {
        lock_position();
      }
    }
    while (position == 2) {
      if(controls::lock_intake()) {
        launch_position();
      }
    }
    while (position == 3) {
      if(controls::launch() || is_rapid_fire) {
        home_position();
      }
    }
  }
}

