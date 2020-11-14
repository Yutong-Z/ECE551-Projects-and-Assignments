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
class Choice:
  represents a choice to go to another page.

Fields (private):
  pageNum: An unsigned int represents number of page to go.
  text: The description of choice.

Methods (Public):
  printChoice: Print the text (parsed from story page file) of choice.
  getChoicePage: get the page number to go of choice.
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

/*
abstract class Page:
  has two concrete subclass: midPage, endPage
  represents a page in story.

Field (private):
  lines: a vector of std::string that contains each line after '#' in a vaild page file.

Field (public):
  currPageNum: a const unsigned int represents the page number of this page.
  prevPages: a vector of std::pair represents a list of pages that can come to this page.
    for each pair, first of pair is the page number of page that can come to this page,
    and second of pair is the choice number that lead the prevous page to this page.

Methods (public):
  printPage: prints the content of this page, and choices or win/lose ending of this page.
  parseNavLine: parses a line from the navigation part of page file and checks format of the line
    and adds the information parsed to this page. (check more detailed introduction in cyoa.cpp)
  checkEnd: check if this page is a end page (page contains WIN/LOSE) or not.
    If it is an end page (endPage dymnamic type instance), returns 1, otherwise, returns 0.
  getNav: gets the choices' page number to go as a vector with the sequence parsed of this page
    or gets a size 1 vector conatins 1 for WIN page and 0 for LOSE page.
    (check more detailed introduction in cyoa.cpp)
  printLines: prints lines field of this page to stdout.
  addLine: adds the input line to lines field's vector. Use for parsing page file.
 */
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
  virtual ~Page() {}  // empty destructor, no need to wirte for rule of three
  virtual void printPage() const = 0;
  virtual void parseNavLine(std::string & line) = 0;
  virtual bool checkEnd() const = 0;
  virtual std::vector<unsigned int> getNav() const = 0;
  void printLines() const;
  void addLine(std::string & line) { lines.push_back(line); }
};

/*
class midPage:
  concrete subcalss of Page.
  represents a page with choices in its navigation part.

Field (private):
  nav: A vector of instance of Choice class, represents all choices the this page has.

Methods (Public):
  As introduced in Page class.
 */
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

/*
class endPage:
  concrete subclass of Page.
  represents a page with WIN or LOSE line in navigation part.

Field (private):
  ifWin: 1 if the navigation part is WIN, 0 if navigation part is LOSE.

Methods (public):
   As introduced in Page class.
 */
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
