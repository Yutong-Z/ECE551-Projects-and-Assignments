#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char ** argv) {
  if (argc == 1) {
    std::string line;
    std::vector<std::string> lines;
    while (getline(std::cin, line)) {
      lines.push_back(line);
    }
    std::sort(lines.begin(), lines.end());
    std::vector<std::string>::iterator it = lines.begin();
    while (it != lines.end()) {
      std::cout << *it << "\n";
      ++it;
    }
  }
  else if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      std::string line;
      std::vector<std::string> lines;
      std::ifstream f(argv[i]);
      if (!f.is_open()) {
        std::cerr << "Can not open file " << argv[i] << std::endl;
        exit(EXIT_FAILURE);
      }
      while (getline(f, line)) {
        lines.push_back(line);
      }
      std::sort(lines.begin(), lines.end());
      std::vector<std::string>::iterator it = lines.begin();
      while (it != lines.end()) {
        std::cout << *it << "\n";
        ++it;
      }
      f.close();
    }
  }
  return EXIT_SUCCESS;
}
