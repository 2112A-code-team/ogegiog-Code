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

void manual_tune_pid();

// {*Name*, {*Function*, *Description*}}
inline const std::map<std::string, std::pair<void(*)(), std::string> > auton_list{
    {"Far Side", {far_side_auton, "Does stuff"}},
    {"Near Side", {near_side_auton, "Does stuff"}},
    {"Skills", {skills, "Does stuff"}}
};

inline std::string selected_auton = "Far Side";

#endif
