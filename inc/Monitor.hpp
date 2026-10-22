#include <array>
#include <fstream>
#include <sstream>
#include <vector>

#include "CpuTimes.hpp"

class Monitor {
 public:
  Monitor(std::string fileName);

  void run(int scanDelay);
  void readCpuTimes(std::vector<CpuTimes>& out);

  double calcUsage(const CpuTimes& previous, const CpuTimes& current);

 private:
  void init(std::string fileName);

 private:
  std::ifstream _statFile;
  std::ofstream _out;
  std::string _line;
  std::istringstream _stringStream;
  std::array<unsigned long long, 8> _cpuTicks{};
  std::vector<CpuTimes> _previousScan;
  std::vector<CpuTimes> _currentScan;
};