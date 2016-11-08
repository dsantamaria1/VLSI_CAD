#include <string>
#include <vector>

#include "Site.h"
#include "Cell.h"

using namespace std;


Site::Site (int x, int y, const string& type) :
	_x (x),
	_y (y),
	_type (type),
	_cellName ("")
{}

void Site::addCell (string cellName) {
	_cellName = cellName; 	
}

