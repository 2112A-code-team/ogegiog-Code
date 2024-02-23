#ifndef _AUTONOMOUS_HPP_
#define _AUTONOMOUS_HPP_

#include "main.hpp"
#include <map>
#include <utility>
#include <array>

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

//no idea why double braces are needed at start and end, but they are

inline const std::array<std::pair<std::string,void(*)()>, 3> auton_list2 {{
  {"Far Side", far_side_auton},
  {"Near Side", near_side_auton},
  {"Skills", skills}
}}; 

inline std::string selected_auton = "Far Side";

#endif
