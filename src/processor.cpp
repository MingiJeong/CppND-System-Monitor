#include "processor.h"
#include "linux_parser.h"

#include <vector>
#include <string>

float Processor::Utilization() {
    const std::vector<std::string> output = LinuxParser::CpuUtilization();
    return std::stof(output.at(1)) / std::stof(output.at(0));
}