#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  // get for pids and set it for processes attributes
  Processor& Cpu();                   
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization() const;         
  long UpTime() const;           
  int TotalProcesses() const;             
  int RunningProcesses() const;             
  std::string Kernel() const;               
  std::string OperatingSystem() const;
  void SetProcessess();     

 private:
  Processor cpu_;
  std::vector<Process> processes_;
};

#endif