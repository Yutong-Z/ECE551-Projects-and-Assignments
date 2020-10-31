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
  std::vector<std::string> * lines;

 public:
  Page(const unsigned int n) : currPageNum(n), lines(new std::vector<std::string>()) {}
  virtual ~Page() { delete lines; }
  virtual void printPage() = 0;
  virtual void parseNavLine(std::string & line) = 0;
  virtual bool checkEnd() = 0;
  void printLines();
  void addLine(std::string & line) { lines->push_back(line); }
};

class midPage : public Page {
 private:
  std::vector<Choice> * nav;

 public:
  midPage(const unsigned int n) : Page(n), nav(new std::vector<Choice>()){};
  virtual ~midPage() { delete nav; }
  virtual void printPage();
  virtual void parseNavLine(std::string & line);
  virtual bool checkEnd() { return 0; }
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
};

/* Functions */
Page * parsePage(std::ifstream & f, const unsigned int pageNum);
unsigned int getPageNum(std::string & pageNumStr);

#endif
