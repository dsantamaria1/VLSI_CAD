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
	vector<string> getCellNames ();
	int calculateHPWL (unordered_map<string, Cell>* cellMap);
	int calculateHPWL();
	void sortXList();
	int getXMin();
	int getXMax();
	vector<int> getXList();
	void buildXList(unordered_map<string, Cell>* cellMap);
	void buildYList(unordered_map<string, Cell>* cellMap);
	friend ostream& operator<< (ostream& os, const Net& net);
private:
	string _name;
	vector<string> _cellNames;
	vector<int> _xList;
	vector<int> _yList;
};

#endif
