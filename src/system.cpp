#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {SetProcessess();}

Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

std::string System::Kernel() const { return LinuxParser::Kernel(); }

float System::MemoryUtilization() const { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() const { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

long int System::UpTime() const { return LinuxParser::UpTime(); }

void System::SetProcessess() {
    vector<int> pid_vec{LinuxParser::Pids()};
    vector<Process> proc_fill;
    // TODO: processes_ only test
    for (auto &id_value : pid_vec) {
        Process new_process;
        new_process.SetPid(id_value);
        proc_fill.push_back(new_process);
    }
    processes_ = proc_fill;
}