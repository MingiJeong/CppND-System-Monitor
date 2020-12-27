#include <string>

#include "format.h"

using std::string;

#define HOUR 3600
#define MIN 60

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
std::string Format::ElapsedTime(long seconds) { 
    std::string hour = std::to_string(seconds / HOUR);
    int second = seconds % HOUR;
    std::string minute = std::to_string(second / MIN);
    std::string sec = std::to_string(second % MIN);
    return TwoDigit(hour) + ":" + TwoDigit(minute) + ":" + TwoDigit(sec); 
}

std::string Format::TwoDigit(std::string &time) {
    if (time.length() < 2) return "0" + time;
    else return time;
}