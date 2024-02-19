#include "main.hpp"
#include <set>
#include <mutex>
#include <utility>
#include <string>


struct Alert {
    enum class Priority {
        LOWEST,
        LOWER,
        NORMAL,
        HIGHER,
        HIGHEST
    };
    Priority priority;
    std::string text;
    mutable int duration;

    Alert(std::string t, Priority p = Priority::NORMAL, int d = 4000) : 
        priority(p), text(t), duration(d)
    {}

    friend bool operator<(const Alert& a, const Alert& b) {
        return a.priority < b.priority;
    }
    friend bool operator==(const Alert& a, const Alert& b) {
        return a.priority == b.priority && a.text == b.text;
    }
};


class AlertList {
    public:
        AlertList() : alert_list(&compare), handled_alert_list(&compare), is_new_alert(false) {}
        void addAlert(Alert newAlert) {
            std::lock_guard<pros::Mutex> lock(alert_list_lock);
            locklessAddAlert(newAlert);
        }
        void addUniqueAlert(Alert newAlert) {
            std::lock_guard<pros::Mutex> lock(alert_list_lock);
            if(locklessHasAlert(newAlert)) {
                locklessAddAlert(newAlert);
            }
        }       
        std::pair<std::string, bool> getCurrentAlert() {
            std::lock_guard<pros::Mutex> lock(alert_list_lock);
            if(alert_list.empty()) {
                return std::pair<std::string, bool>("", false);
            }
            static int lastTime = pros::millis();
            static Alert lastAlert = *alert_list.begin();
            auto it = alert_list.begin();
            if(lastAlert == *it) {
                it->duration -= pros::millis() - lastTime;
            }
            lastAlert = *it;
            if(it->duration <= 0) {
                handled_alert_list.insert(*it);
                alert_list.erase(it);
            }
            lastTime = pros::millis();
            auto ret_val = std::pair<std::string, bool>(alert_list.begin()->text, is_new_alert);
            is_new_alert = false;
            return ret_val;
        }
    private:
        void locklessAddAlert(Alert newAlert) {
            auto it = alert_list.begin();
            alert_list.insert(newAlert);
            if(alert_list.begin() != it) {
                is_new_alert = true;
            }
        } 
        bool locklessHasAlert(Alert newAlert) {
            auto start = alert_list.lower_bound(newAlert);
            auto end = alert_list.upper_bound(newAlert);
            for(auto i = start; i != end; i++) {
                if(*i == newAlert) {
                    return true;
                }
            }
            auto start2 = handled_alert_list.lower_bound(newAlert);
            auto end2 = handled_alert_list.upper_bound(newAlert);
            for(auto i = start2; i != end2; i++) {
                if(*i == newAlert) {
                    return true;
                }
            }

            return false;
        }
        pros::Mutex alert_list_lock;
        static bool compare(Alert a, Alert b) {
            return a.priority < b.priority;
        }
        std::multiset<Alert, decltype(&compare)> alert_list;
        std::multiset<Alert, decltype(&compare)> handled_alert_list;
        bool is_new_alert;
};

inline AlertList controller_alerts;