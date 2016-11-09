#ifndef SITE_H
#define SITE_H

#include <string>
#include "Cell.h"

using namespace std;


class Site {
public:
	Site (int x, int y, const string& type);
	void addCell (string cellName);
private:
	int _x;
	int _y;
	string _type;
	string _cellName;
};

#endif
