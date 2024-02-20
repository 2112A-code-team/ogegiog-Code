#ifndef _AUTONOMOUS_HPP_
#define _AUTONOMOUS_HPP_

#include "main.hpp"
#include <map>
#include <utility>

void far_side_auton();

void near_side_auton();

void skills();

void tune_pid();

void set_up_auton_selector();

// {*Name*, {*Function*, *Description*}}
inline std::map<std::string, std::pair<void(*)(), std::string> > auton_list{
    {"Far Side", {far_side_auton, "Does stuff"}},
    {"Near Side", {near_side_auton, "Does stuff"}},
    {"Skills", {skills, "Does stuff"}},
    {"Tune PID", {tune_pid, "Does stuff"}}
};

inline std::string selected_auton = "Tune PID";

#endif
