#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::cout;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value; // if not found, return blank value
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename); // initialization 
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value, unit_capacity;
  float total_value, used_value, ratio;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unit_capacity) {
        if (key == "MemTotal") {
          total_value = std::stof(value) ;
        }
        if (key == "MemFree") {
          used_value = total_value - std::stof(value);
          ratio = used_value / total_value;
          return ratio;
        }
      }
    }
  }
  return ratio; // if not found, return blank value
}  

long LinuxParser::UpTime() {   
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename); // initialization 
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return std::stol(uptime);
}

long LinuxParser::Jiffies() {
  string cpu; // only aggregate cpu (first line)
  string line;
  CPUStates state; // class instantiation
  std::ifstream stream(kProcDirectory + kStatFilename); // initialization 
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> state.kUser_ >> state.kNice_ >> state.kSystem
    >> state.kIdle_ >> state.kIOwait_ >> state.kIRQ_ >> state. kSoftIRQ_
    >> state.kSteal_ >> state.kGuest_ >> state.kGuestNice_;

  }
  return state.sigma();
}


float LinuxParser::ActiveJiffies(int pid) {
  string line;
  ProcessStates state;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename); // initialization 
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> state.id_ >> state.comm_ >> state.ppid_ >> state.pgrp_ >>
    state.session_ >> state.tty_nr >> state.tpg_id >> state.flags_ >> state.minflt_ >>
    state.cminflt_ >> state.cmajflt_ >> state.majflt_ >> state.cmajflt_ >>
    state.utime_ >> state.stime_ >> state.cutime_ >> state.cstime_ >> state.priority_ >>
    state.nice_ >> state.num_thread_ >> state.itrealvalue_ >> state.starttime_ ;  

    long total_time = state.active();
    float seconds = UpTime() - (std::stof(state.starttime_) / sysconf(_SC_CLK_TCK));
    float cpu_usage = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
    return cpu_usage;
  }
  return 0;
}


long LinuxParser::ActiveJiffies() {
  string cpu; // only aggregate cpu (first line)
  string line;
  CPUStates state; // class instantiation
  std::ifstream stream(kProcDirectory + kStatFilename); // initialization 
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> state.kUser_ >> state.kNice_ >> state.kSystem
    >> state.kIdle_ >> state.kIOwait_ >> state.kIRQ_ >> state. kSoftIRQ_
    >> state.kSteal_ >> state.kGuest_ >> state.kGuestNice_;

  }
  return state.active();
}

long LinuxParser::IdleJiffies() {
  string cpu; // only aggregate cpu (first line)
  string line;
  CPUStates state; // class instantiation
  std::ifstream stream(kProcDirectory + kStatFilename); // initialization 
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> state.kUser_ >> state.kNice_ >> state.kSystem
    >> state.kIdle_ >> state.kIOwait_ >> state.kIRQ_ >> state. kSoftIRQ_
    >> state.kSteal_ >> state.kGuest_ >> state.kGuestNice_;

  }
  return state.idle();
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_util;
  cpu_util.push_back(std::to_string(Jiffies()));
  cpu_util.push_back(std::to_string(ActiveJiffies()));
  cpu_util.push_back(std::to_string(IdleJiffies()));

  return cpu_util;
}


int LinuxParser::TotalProcesses() {
  string line;
  string key, value;
  int total_running;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          total_running = std::stof(value);
          return total_running;
        }
      }
    }
  }
  return total_running; // if not found, return blank value
}  

int LinuxParser::RunningProcesses() { 
  string line;
  string key, value;
  int current_running;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          current_running = std::stof(value);
          return current_running;
        }
      }
    }
  }
  return current_running; // if not found, return blank value
} 
  

string LinuxParser::Command(int pid) {
  std::string line{"NONE"};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

string LinuxParser::Ram(int pid) {
string line;
  string key, value;
  string ram;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          ram = to_string(static_cast<int>(std::stoi(value) * 0.001));
          return ram;
        }
      }
    }
  }
  return ram; // if not found, return blank value
} 
  

string LinuxParser::Uid(int pid) {
  string line;
  string key, value;
  string uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
        }
      }
    }
  }
  return uid; // if not found, return blank value
} 


string LinuxParser::User(int pid) {
  string line;
  string key;
  string dummy1;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> dummy1 >> value) {
        if (value == Uid(pid)) {
          return key;
        }
      }
    }
  }
  return key; // if not found, return blank value
}

long LinuxParser::UpTime(int pid) {
  string line;
  ProcessStates state;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename); // initialization 
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> state.id_ >> state.comm_ >> state.ppid_ >> state.pgrp_ >>
    state.session_ >> state.tty_nr >> state.tpg_id >> state.flags_ >> state.minflt_ >>
    state.cminflt_ >> state.cmajflt_ >> state.majflt_ >> state.cmajflt_ >>
    state.utime_ >> state.stime_ >> state.cutime_ >> state.cstime_ >> state.priority_ >>
    state.nice_ >> state.num_thread_ >> state.itrealvalue_ >> state.starttime_ ;  
    return LinuxParser::UpTime() - std::stol(state.starttime_) / sysconf(_SC_CLK_TCK);
  }
  return 0;
}