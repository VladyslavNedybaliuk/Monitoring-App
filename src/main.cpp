#include <iostream>
#include <string>

#include "Monitor.hpp"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: <seconds> <file_name>" << std::endl;
    return 1;
  }
  int seconds = std::atoi(argv[1]);
  std::string fileName = argv[2];

  Monitor monitor(fileName);

  while (true) {
    monitor.run(seconds);
  }
}