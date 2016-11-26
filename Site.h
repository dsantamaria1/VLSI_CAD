#ifndef SITE_H
#define SITE_H

#include <string>
#include <iostream>

#include "Cell.h"

using namespace std;


class Site {
public:
	Site (int x = 0, int y = 0, const string& type = "", const string& cellName = "");
	Site (const Site& site);
	void addCell (string cellName);
	string getType ();
	string getCellName ();
	friend ostream& operator<<(ostream& os, const Site& site);
private:
	int _x;
	int _y;
	string _type;
	string _cellName;
};

#endif
