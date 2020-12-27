#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() const { return pid_; }

void Process::SetPid(int pid) {pid_ = pid;}

float Process::CpuUtilization() const { return LinuxParser::ActiveJiffies(Pid());}

string Process::Command() const { return LinuxParser::Command(Pid());}

string Process::Ram() const { return LinuxParser::Ram(Pid());}

string Process::User() const { return LinuxParser::User(Pid());}

long int Process::UpTime() const { return LinuxParser::UpTime(Pid());}

bool Process::operator<(Process const& a) const { return this->CpuUtilization() < a.CpuUtilization() ? true : false; }