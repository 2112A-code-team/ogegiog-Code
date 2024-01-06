#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_
#include "main.hpp" 
//#include <atomic>


/**
 * Initialize all the devices needed (motors, sensors, etc.)
*/
inline pros::Motor left_front(1, pros::E_MOTOR_GEAR_BLUE);
inline pros::Motor left_back(2, pros::E_MOTOR_GEAR_BLUE);
inline pros::Motor right_front(3, pros::E_MOTOR_GEAR_BLUE);
inline pros::Motor right_back(4, pros::E_MOTOR_GEAR_BLUE);
inline pros::Motor_Group left_wheels({left_front, left_back});
inline pros::Motor_Group right_wheels({right_front, right_back});
inline pros::Motor cata_left(7, pros::E_MOTOR_GEAR_RED);
inline pros::Motor cata_right(8, pros::E_MOTOR_GEAR_RED);
inline pros::Motor_Group cata_motors({cata_left, cata_right});
inline pros::Motor intake_motor(9, pros::E_MOTOR_GEAR_GREEN);
inline pros::Controller master(CONTROLLER_MASTER);
inline pros::Rotation cata_sensor(10);
inline pros::Imu inertial(11);

inline Drive chassis (
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  {1, -2, 3}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  ,{-4, 5, -6}

  // IMU Port
  ,11

  // Wheel Diameter
  ,3.25

  // Cartridge RPM
  ,600

  // External Gear Ratio (MUST BE DECIMAL)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,2.333
);

// NOTE: https://pros.cs.purdue.edu/v5/tutorials/topical/multitasking.html#synchronization

inline double robot_x(0.0);
inline double robot_y(0.0);

inline bool is_arcade = false;

namespace controls
{
    inline bool intake_forward() {return master.get_digital(DIGITAL_L1);}
    inline bool intake_backward() {return master.get_digital(DIGITAL_L2);}
    inline bool launch() {return master.get_digital(DIGITAL_B);}
    inline bool lock_intake() {return master.get_digital(DIGITAL_DOWN);}
    inline bool rapid_fire() {
        static bool is_rapid_fire = false;
        static pros::Task button_check([&] {
            while (true) {
                if(master.get_digital_new_press(DIGITAL_X)) is_rapid_fire ^= true;
                pros::delay(50);
            }
        });
        return is_rapid_fire;
    }
    inline bool reverse_joysticks() {
        static bool is_reversed = false;
        static pros::Task button_check([&] {
            while (true) {
                if(master.get_digital_new_press(DIGITAL_Y)) is_reversed ^= true;
                pros::delay(50);
            }
        });
        return is_reversed;
    }
    inline bool slow_joysticks() {return master.get_digital(DIGITAL_R1);}
    //inline bool wing() {return master.get_digital(DIGITAL_R1);}
} // namespace controls


#endif