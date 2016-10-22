#include <iostream>
#include <stdlib.h>
#include "Cell.h"

class Cell{
  string name;
  int x;
  int y;
  string type;
  bool fixed;

  Cell::Cell(string name, int x, int y, string type, bool fixed){
    this->name = name;
    this->x = x;
    this->y = y;
    this->type = type;
    this->fixed = fixed;
  }
  Cell::~Cell(void){
    cout<< this->name << "is being deleted" << endl;
  }
};
