#ifndef NET_H
#define NET_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "Cell.h"

using namespace std;

class Net {
public:
	Net (string name = "", vector<string> cellNames = vector<string>());
	Net (const Net& net);
	string getName ();
	int getXMin ();
	int getXMax ();
	int getYMin ();
	int getYMax ();
	vector<string> getCellNames ();
	void setBoundingBox (int x_min, int x_max, int y_min, int y_max); 
	void findBoundingBox (unordered_map<string, Cell>* cellMap);
	int getHPWL ();
	friend ostream& operator<< (ostream& os, const Net& net);
private:
	string _name;
	int _x_min;
	int _x_max;
	int _y_min;
	int _y_max;
	vector<string> _cellNames;	
};

#endif
