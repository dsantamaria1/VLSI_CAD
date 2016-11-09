#ifndef CELL_H
#define CELL_H

#include <vector>
#include <string>

using namespace std;


class Cell {
public:
	Cell (const string& name, const string& type, int x, int y, 
		const string& fixed);
	string getName ();
	string getType ();
	int getX ();
	int getY ();
	bool isFixed ();
	void addNets (vector<string> netNames);	
	void printData ();
private:
	string _name;
	string _type;
	int _x;
	int _y;
	string _fixed;
	vector<string> _netNames;
};

#endif

