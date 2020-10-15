#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char ** argv) {
  std::string line;
  std::vector<std::string> lines;
  if (argc == 1) {
    while (getline(std::cin, line)) {
      lines.push_back(line);
    }
  }
  else {
    for (int i = 1; i < argc; i++) {
      std::ifstream f(argv[i]);
      if (!f.is_open()) {
        std::cerr << "Can not open file " << argv[i] << std::endl;
      }
      while (getline(f, line)) {
        lines.push_back(line);
      }
      f.close();
    }
  }
  std::sort(lines.begin(), lines.end());
  std::vector<std::string>::iterator it = lines.begin();
  while (it != lines.end()) {
    std::cout << *it << "\n";
    ++it;
  }
}
