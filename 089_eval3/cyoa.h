#ifndef __CYOA_H_
#define __CYOA_H_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/*
class represents a choice to go to another page
Fields:
  pageNum: A string literal represents number of page to go.
  text: The description of choice.
 */

class Choice {
 private:
  const std::string pageNum;
  const std::string text;

 public:
  Choice(const std::string & p, const std::string & t) : pageNum(p), text(t){};
  void printChoice() { std::cout << ". " << text << "\n"; }
};

class Page {
 public:
  std::string currPageNum;
  std::vector<std::string> * lines;

 public:
  Page(std::string & n) : currPageNum(n), lines(new std::vector<std::string>()) {}
  virtual ~Page() { delete lines; }
  virtual void printPage() = 0;
  virtual void parseNavLine(std::string & line) = 0;
  void printLines() {
    std::vector<std::string>::iterator it = lines->begin();
    while (it != lines->end()) {
      std::cout << *it << "\n";
      ++it;
    }
  }
  void addLine(std::string & line) { lines->push_back(line); }
};

class midPage : public Page {
 private:
  std::vector<Choice> * nav;

 public:
  midPage(std::string & n) : Page(n), nav(new std::vector<Choice>()){};
  virtual ~midPage() { delete nav; }
  virtual void printPage() {
    printLines();
    std::cout << "\n"
              << "What would you like to do?\n\n";
    std::vector<Choice>::iterator it = nav->begin();
    unsigned int i = 1;
    while (it != nav->end()) {
      std::cout << i;
      it->printChoice();
      ++it;
      i++;
    }
  }
  virtual void parseNavLine(std::string & line) {
    size_t colonIdx = line.find(':');
    if (colonIdx == std::string::npos) {
      std::cerr << "No ':' found in this choice navigation line below:\n"
                << line << std::endl;
      exit(EXIT_FAILURE);
    }
    // DO I NEED TO CHECK THIS NUM??
    std::string pageNum = line.substr(0, colonIdx);
    if (pageNum[0] == '-') {
      std::cerr << "Negative page number in choice: " << pageNum << std::endl;
      exit(EXIT_FAILURE);
    }
    std::string text = line.substr(colonIdx + 1);
    nav->push_back(Choice(pageNum, text));
  }
};

class endPage : public Page {
 private:
  bool ifWin;

 public:
  endPage(std::string & n, bool b) : Page(n), ifWin(b){};
  virtual ~endPage() {}
  virtual void printPage() {
    printLines();
    std::cout << "\n";
    printStatus();
  }
  virtual void parseNavLine(std::string & line) {
    if (line.compare("WIN") == 0) {
      ifWin = 1;
    }
    else if (line.compare("LOSE") == 0) {
      ifWin = 0;
    }
    else {
      std::cerr << "Input line of parseStatus is not WIN\n or LOSE\n" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  void printStatus() {
    if (ifWin == 1) {
      std::cout << "Congratulations! You have won. Hooray!" << std::endl;
    }
    else {
      std::cout << "Sorry, you have lost. Better luck next time!" << std::endl;
    }
  }
};

/* Functions */
Page * parsePage(std::ifstream & f, std::string & pageNum);
unsigned int getPageNum(std::string pageNumStr);

#endif
