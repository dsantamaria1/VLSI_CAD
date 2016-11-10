#ifndef CELL_H
#define CELL_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;


class Cell {
public:
	Cell (const string& name = "", const string& type = "", int x = 0, int y = 0, 
		const string& fixed = "");
	Cell (const Cell& cell);
	string getName ();
	string getType ();
	int getX ();
	int getY ();
	bool isFixed ();
	void addNets (vector<string> netNames);	
	vector<string> getNetNames ();
	void printData ();
	friend ostream& operator<< (ostream& os, const Cell& cell);
private:
	string _name;
	string _type;
	int _x;
	int _y;
	string _fixed;
	vector<string> _netNames;
};

#endif

