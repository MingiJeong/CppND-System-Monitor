#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel() ;

// CPU
class CPUStates {
  public:
  std::string kUser_;
  std::string kNice_;
  std::string kSystem;
  std::string kIdle_;
  std::string kIOwait_;
  std::string kIRQ_;
  std::string kSoftIRQ_;
  std::string kSteal_;
  std::string kGuest_;
  std::string kGuestNice_;

  long sigma() {
   return std::stol(kUser_) + std::stol(kNice_) + std::stol(kSystem) + std::stol(kIdle_) + std::stol(kIOwait_) + std::stol(kIRQ_)
  + std::stol(kSoftIRQ_) + std::stol(kSteal_) + std::stol(kGuest_) + std::stol(kGuestNice_);
  }

  long active() {
   return std::stol(kUser_) + std::stol(kNice_) + std::stol(kSystem) + std::stol(kIRQ_)
  + std::stol(kSoftIRQ_) + std::stol(kSteal_);
  }

  long idle() {
    return std::stol(kIdle_) + std::stol(kIOwait_);
  }
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
float ActiveJiffies(int pid);
long IdleJiffies();

// Processes
class ProcessStates {
  public:
  std::string id_; // 1
  std::string comm_; // 2
  std::string state_; // 3
  std::string ppid_; // 4
  std::string pgrp_; // 5
  std::string session_; // 6
  std::string tty_nr; // 7
  std::string tpg_id; // 8
  std::string flags_; // 9
  std::string minflt_; // 10
  std::string cminflt_; // 11
  std::string majflt_; // 12 
  std::string cmajflt_;// 13
  std::string utime_; // 14
  std::string stime_; // 15
  std::string cutime_; // 16
  std::string cstime_; // 17
  std::string priority_; // 18

  std::string nice_; // 19
  std::string num_thread_; // 20
  std::string itrealvalue_; // 21
  std::string starttime_; // 22
  
  long active() {
   return std::stol(utime_) + std::stol(stime_) + std::stol(cutime_) + std::stol(cstime_);
  }
};

std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif