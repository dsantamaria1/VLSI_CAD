#include <iostream>
#include <stdlib.h>
#include "Cell.hh"

class Cell{
  string name;
  int x;
  int y;
  string type;
  string fixed;

  Cell::Cell(string name, int x, int y, string type, string fixed){
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
