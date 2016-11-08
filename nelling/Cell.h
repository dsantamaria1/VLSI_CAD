#ifndef CELL_H
#define CELL_H

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
	void addNeighbors (vector <Cell*> neighbors);	
	//vector <Cell*> getNeighbors ();
	void printData ();
private:
	const string _name;
	const string _type;
	int _x;
	int _y;
	const string _fixed;
	const Cell* _neighbors;
};

#endif

