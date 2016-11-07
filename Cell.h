#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

class Cell{

private:
  string name;
  int x;
  int y;
  string type;
  string fixed;

public:
  Cell(string name, int x, int y, string type, string fixed){};
  virtual ~Cell(){};
};
