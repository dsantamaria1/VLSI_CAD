#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

class Cell{
public:
  Cell(string name, int x, int y, string type, string fixed);
  ~Cell(void);

private:
  string name;
  int x;
  int y;
  string type;
  string fixed;
};
