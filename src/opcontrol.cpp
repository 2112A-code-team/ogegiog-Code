#include "main.hpp"
#include <cmath>
#include <atomic>
#include <mutex>

double curve_joystick(double joy_val, double curve) {
  double abs_val = std::abs(joy_val) / 127;
  return joy_val * std::pow(abs_val, curve);
}

double get_joystick(pros::controller_analog_e_t joystick, double curve = 1,
                    double deadzone = 5) {
  double joystick_val = master.get_analog(joystick);
  if (std::abs(joystick_val) >= deadzone) {
    joystick_val = curve_joystick(joystick_val, curve);
  } else {
    joystick_val = 0;
  }
  return joystick_val;
}




void update_controller() {
  //int last_warning_index = 0;
  //int loop_time = 0;
  master.clear();
  pros::delay(500);
  while(true) {
    //this variable holds the text to print
    // (c++ style strings allow using + to add strings)
    std::string text;

    text = "TV: ";
    if(controls::fvw_is_reversed()) text += "-";
    text += std::to_string(flywheel_velocity) + " ";
    master.print(0, 0, text.c_str());
    pros::delay(100);
    int real_vel = static_cast<int>( flywheel.get_actual_velocity() );
    text = "AV: " + std::to_string(real_vel) + "     ";
    master.print(0, 7, text.c_str());
    pros::delay(100);
    text = "Wing Count: " + std::to_string(wing_count);
    master.print(1, 0, text.c_str());
    pros::delay(100);
    bool newAlert = false;
    std::tie(text, newAlert) = controller_alerts.getCurrentAlert();
    if(newAlert) {
      master.rumble("...");
      pros::delay(100);
    }
    master.print(2, 0, text.c_str());
    pros::delay(100);
    //loop_time += 100;
  }
}

void endgame_timer() {
  pros::delay(75 * 1000);
  controller_alerts.addAlert(Alert("30s left!!", Alert::Priority::HIGHEST));
  pros::delay(15 * 1000);
  controller_alerts.addAlert(Alert("15s left!!", Alert::Priority::HIGHEST));
}


/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  pros::Task controller_task(update_controller);
  pros::Task endgame_task(endgame_timer);

  while(true) {

    int time = 0;
    //test auton
    while(controls::fvw_up() && controls::fvw_down()
      && controls::lift_up() && controls::lift_down()) {
        pros::delay(10);
        time += 10;
        if(time == 1000) {
          controller_alerts.addAlert(Alert("PID tune in 1s!!!", Alert::Priority::HIGHEST, 900));
        }
        if(time == 2000) {
          master.rumble(".-.-");
          pros::delay(1000);
          controller_task.suspend();
          manual_tune_pid();
          controller_task.resume();
        }
        /*if(time == 1000) {
          controller_alerts.addAlert(Alert("Auton test in 1s!!!", Alert::Priority::HIGHEST, 1000));
        }
        if(time == 2000) {
          controller_alerts.addAlert(Alert("Skills: A, Regular: X", Alert::Priority::HIGHEST, 1000));
          bool skills = false;
          bool regular = false;
          while(!skills && !regular) {
            skills = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A);
            regular = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X);
          }
          pros::Task auton_tesk_task(autonomous);
          if (skills) {
            controller_alerts.addAlert(Alert("Skills", Alert::Priority::HIGHEST));
            pros::delay(60 * 1000);
          } else {
            controller_alerts.addAlert(Alert("Regular Auton", Alert::Priority::HIGHEST));
            pros::delay(15 * 1000);
          }
          auton_tesk_task.suspend();
        }*/
      }

    //wing control
    if(controls::wing_is_out()) {
      wings.set_value(1);
    } else {
      wings.set_value(0);
    }

    // flywheel arm control
    if(controls::fly_arm_forward()) {
      flywheel_arm.move_velocity(50);
    } else if(controls::fly_arm_backward()) {
      flywheel_arm.move_velocity(-50);
    } else {
      flywheel_arm.brake();
    }

    // flywheel control
    if(controls::fvw_up()) flywheel_velocity++;
    if(controls::fvw_down()) flywheel_velocity--;
    if(flywheel_velocity < 0) flywheel_velocity = 0;
    if(flywheel_velocity > 200) flywheel_velocity = 600;
    if(controls::flywheel_is_on() && controls::fvw_is_reversed()) {
      flywheel.move_velocity(-flywheel_velocity);
    } else if (controls::flywheel_is_on()) {
      flywheel.move_velocity(flywheel_velocity);
    } else {
      flywheel.brake();
    }

    // lift control
    if(controls::lift_up()) {
      lift.move_velocity(100);
    } else if(controls::lift_down()) {
      lift.move_velocity(-100);
    } else {
      lift.brake();
      if(lift.get_positions()[0] < 360) {
        lift.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
        led.set_value(1);
      } else {
        lift.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
        led.set_value(0);
      }
    }

    // drive control
    double forward_speed = get_joystick(ANALOG_RIGHT_X);
    double turn_speed = get_joystick(ANALOG_LEFT_Y);
    left_wheels.move(forward_speed + turn_speed);
    right_wheels.move(-1 * (forward_speed - turn_speed)); //not sure why -1 is needed but it is
    pros::delay(10);
  }
}
