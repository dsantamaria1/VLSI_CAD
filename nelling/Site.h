#ifndef SITE_H
#define SITE_H

#include <string>
#include "Cell.h"

using namespace std;


class Site {
public:
	Site (int x, int y, const string& type);
	void addCell (Cell cell);
private:
	const int _x;
	const int _y;
	const string _type;
	Cell* _siteCell;
};

#endif
