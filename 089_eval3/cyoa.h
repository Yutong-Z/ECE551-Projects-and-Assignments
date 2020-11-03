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
  pageNum: An unsigned int represents number of page to go.
  text: The description of choice.
 */
class Choice {
 private:
  const unsigned int pageNum;
  const std::string text;

 public:
  Choice(const unsigned int p, const std::string & t) : pageNum(p), text(t){};
  void printChoice() { std::cout << ". " << text << "\n"; }
  unsigned int getChoicePage() { return pageNum; }
};

class Page {
 public:
  const unsigned int currPageNum;
  std::vector<std::string> lines;

 public:
  Page(const unsigned int n) : currPageNum(n), lines(std::vector<std::string>()) {}
  virtual ~Page() {}
  virtual void printPage() = 0;
  virtual void parseNavLine(std::string & line) = 0;
  virtual bool checkEnd() = 0;
  virtual std::vector<unsigned int> getNav();
  void printLines();
  void addLine(std::string & line) { lines.push_back(line); }
};

class midPage : public Page {
 private:
  std::vector<Choice> nav;

 public:
  midPage(const unsigned int n) : Page(n), nav(std::vector<Choice>()){};
  virtual ~midPage() {}
  virtual void printPage();
  virtual void parseNavLine(std::string & line);
  virtual bool checkEnd() { return 0; }
  virtual std::vector<unsigned int> getNav() {
    std::vector<unsigned int> choices;
    std::vector<Choice>::iterator it = nav.begin();
    while (it != nav.end()) {
      choices.push_back(it->getChoicePage());
    }
    return choices;
  }
};

class endPage : public Page {
 private:
  bool ifWin;

 public:
  endPage(const unsigned int n, bool b) : Page(n), ifWin(b){};
  virtual ~endPage() {}
  virtual void printPage();
  virtual void parseNavLine(std::string & line);
  virtual bool checkEnd() { return 1; }
  virtual std::vector<unsigned int> getNav() {
    std::vector<unsigned int> choices;
    if (ifWin) {
      choices.push_back(1);
    }
    else {
      choices.push_back(0);
    }
    return choices;
  }
};

/* Functions */
Page * parsePage(std::ifstream & f, const unsigned int pageNum);
unsigned int getPageNum(std::string & pageNumStr);

#endif
