#ifndef __CYOA_H_
#define __CYOA_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/*
class represents a choice to go to another page
Fields:
  seqNum: The sequence number of choice.
  pageNum: A string literal represents number of page to go.
  text: The description of choice.
 */
class choice {
 private:
  unsigned int
      seqNum;  //*** Delete if not necessary // larger than 0, but no upper bound limit
  const std::string pageNum;
  const std::string text;

 public:
  choice(unsigned int n, const std::string & p, const std::string & t) :
      seqNum(n),
      pageNum(p),
      text(t){};
  void printChoice() { std::cout << seqNum << ". " << text << "\n"; }
};

class page {
 public:
  unsigned int currPageNum;
  std::vector<std::string> * lines;

 public:
  page(unsigned int n) : currPageNum(n), lines(new std::vector<std::string>()) {}
  virtual ~page() { delete lines; }
  virtual void printPage() = 0;
  void printLines() {
    std::vector<std::string>::iterator it = lines->begin();
    while (it != lines->end()) {
      std::cout << *it;
      ++it;
    }
  }
  // TO DO!!! add line
};

class midPage : public page {
 private:
  std::vector<choice> * nav;

 public:
  midPage(unsigned int n) : page(n), nav(new std::vector<choice>()){};
  virtual ~midPage() { delete nav; }
  virtual void printPage(){
      // TO DO !!!
  };
};

class endPage : public page {
 private:
  bool ifWin;

 public:
  endPage(unsigned int n, bool b) : page(n), ifWin(b){};
  virtual ~endPage() {}
  virtual void printPage() {
    // TO DO!!
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
#endif
