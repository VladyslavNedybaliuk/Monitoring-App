#include "Monitor.hpp"

#include <unistd.h>

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>

Monitor::Monitor(std::string fileName) { init(fileName); }

void Monitor::init(std::string fileName) {
  _statFile.open("/proc/stat");
  _line.reserve(256);
  _stringStream.str(std::string(256, ' '));
  _previousScan.reserve(64);
  _currentScan.reserve(64);

  _out.open(fileName);
  if (!_out) std::cerr << "Error open " << fileName << std::endl;
}

void Monitor::run(int scanDelay) {
  readCpuTimes(_previousScan);
  sleep(scanDelay <= 1 ? 1 : scanDelay);
  readCpuTimes(_currentScan);

  for (size_t i = 0; i < _previousScan.size(); i++) {
    if (i == 0) {
      std::cout << "Total: ";
      _out << "Total: ";
    } else {
      std::cout << "Core " << i - 1 << ": ";
      _out << "Core " << i - 1 << ": ";
    }
    const double usage = calcUsage(_previousScan[i], _currentScan[i]);

    std::cout << usage << "%\n";
    _out << usage << "%\n";
    _out.flush();
  }
}

void Monitor::readCpuTimes(std::vector<CpuTimes>& out) {
  out.clear();
  _statFile.clear();
  _statFile.seekg(0);

  while (std::getline(_statFile, _line)) {
    if (_line.compare(0, 3, "cpu") != 0) break;

    _cpuTicks = {};
    _stringStream.clear();
    _stringStream.str(_line);

    std::string name;
    _stringStream >> name;

    for (size_t i = 0; i < _cpuTicks.size(); ++i) {
      _stringStream >> _cpuTicks[i];
    }

    CpuTimes times;
    times.idle = _cpuTicks[3] + _cpuTicks[4];
    times.total = _cpuTicks[0] + _cpuTicks[1] + _cpuTicks[2] + _cpuTicks[3] +
                  _cpuTicks[4] + _cpuTicks[5] + _cpuTicks[6] + _cpuTicks[7];
    out.push_back(times);
  }
}

double Monitor::calcUsage(const CpuTimes& previous, const CpuTimes& current) {
  const auto dTotal = current.total - previous.total;
  const auto dIdle = current.idle - previous.idle;
  if (dTotal == 0) return 0.0;

  const double usage =
      (1.0 - static_cast<double>(dIdle) / static_cast<double>(dTotal)) * 100.0;

  return std::round(usage * 10.0) / 10.0;
}