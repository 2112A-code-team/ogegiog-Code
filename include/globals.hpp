#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_
#include "main.hpp" 
#include <deque>


/**
 * Initialize all the devices needed (motors, sensors, etc.)
*/
inline pros::Motor left_front(1, pros::E_MOTOR_GEAR_BLUE);
inline pros::Motor left_back(2, pros::E_MOTOR_GEAR_BLUE);
inline pros::Motor right_front(9, pros::E_MOTOR_GEAR_BLUE);
inline pros::Motor right_back(10, pros::E_MOTOR_GEAR_BLUE);
inline pros::Motor_Group left_wheels({left_front, left_back});
inline pros::Motor_Group right_wheels({right_front, right_back});
inline pros::Motor lift_right(11, pros::E_MOTOR_GEAR_RED);
inline pros::Motor lift_left(20, pros::E_MOTOR_GEAR_RED);
inline pros::Motor_Group lift({lift_left, lift_right});
inline pros::Motor flywheel_arm(19, pros::E_MOTOR_GEAR_GREEN);
inline pros::Motor flywheel(12, pros::E_MOTOR_GEAR_GREEN);
inline pros::ADIDigitalOut wings('H');
inline pros::Controller master(CONTROLLER_MASTER);
inline pros::Imu inertial(11);

inline pros::Mutex hot_motor_list_lock;
inline std::deque<std::string> hot_motor_list;

namespace controls
{
    inline bool fly_arm_forward() {return master.get_digital(DIGITAL_L2);}
    inline bool fly_arm_backward() {return master.get_digital(DIGITAL_L1);}
    inline bool lift_up() {return master.get_digital(DIGITAL_R1);}
    inline bool lift_down() {return master.get_digital(DIGITAL_R2);}
    inline bool flywheel_is_on() {
        static bool is_on = false;
        if(master.get_digital_new_press(DIGITAL_B)) is_on = !is_on;
        return is_on;
    }
    inline bool wing_is_out() {
        static bool is_out = false;
        if(master.get_digital_new_press(DIGITAL_LEFT)) is_out = !is_out;
        return is_out;
    }
    inline bool fvw_up() {return master.get_digital(DIGITAL_UP);}
    inline bool fvw_down() {return master.get_digital(DIGITAL_DOWN);}
    inline bool fvw_is_reversed() {
        static bool is_reversed = false;
        if(master.get_digital_new_press(DIGITAL_Y)) is_reversed = !is_reversed;
        return is_reversed;
    }
} // namespace controls


#endif