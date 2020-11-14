#ifndef __CYOA_H_
#define __CYOA_H_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>  //std::pair
#include <vector>

/*
class Choice: represents a choice to go to another page
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
  void printChoice() const { std::cout << text << "\n"; }
  unsigned int getChoicePage() const { return pageNum; }
};

class Page {
 private:
  std::vector<std::string> lines;

 public:
  const unsigned int currPageNum;
  std::vector<std::pair<unsigned int, unsigned int> > prevPages;

 public:
  Page(const unsigned int n) :
      lines(std::vector<std::string>()),
      currPageNum(n),
      prevPages(std::vector<std::pair<unsigned int, unsigned int> >()) {}
  virtual ~Page() {}
  virtual void printPage() const = 0;
  virtual void parseNavLine(std::string & line) = 0;
  virtual bool checkEnd() const = 0;
  virtual std::vector<unsigned int> getNav() const = 0;
  void printLines() const;
  void addLine(std::string & line) { lines.push_back(line); }
};

class midPage : public Page {
 private:
  std::vector<Choice> nav;

 public:
  midPage(const unsigned int n) : Page(n), nav(std::vector<Choice>()){};
  virtual void printPage() const;
  virtual void parseNavLine(std::string & line);
  virtual bool checkEnd() const { return 0; }
  virtual std::vector<unsigned int> getNav() const;
};

class endPage : public Page {
 private:
  bool ifWin;

 public:
  endPage(const unsigned int n, bool b) : Page(n), ifWin(b){};
  virtual void printPage() const;
  virtual void parseNavLine(std::string & line);
  virtual bool checkEnd() const { return 1; }
  virtual std::vector<unsigned int> getNav() const;
};

/* Functions */
Page * parsePage(std::ifstream & f, const unsigned int pageNum);
unsigned int getNumFromStr(const std::string & pageNumStr);

std::string getFileName(const unsigned int pageNum, const char * directory);
std::vector<Page *> readPages(const char * directory);
void deletePages(std::vector<Page *> & pages);
void checkReference(const std::vector<Page *> & pages);
void playCyoa(const std::vector<Page *> & pages);

std::set<Page *> getReachableSet(const std::vector<Page *> & pages);
void printUnreach(const std::set<Page *> & reachPages, const unsigned int totalPageNum);

void fillPrevs(const std::vector<Page *> & pages, const std::set<Page *> & reachPage);
unsigned int findWinPage(const std::set<Page *> & reachPages);
std::vector<std::pair<unsigned int, unsigned int> > findWinPath(
    const std::vector<Page *> & pages,
    const unsigned int winPageNum);
void printWinPath(const std::vector<std::pair<unsigned int, unsigned int> > & winPath);

#endif
