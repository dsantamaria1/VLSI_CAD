#include <string>
#include <vector>
#include <unordered_map>

#include "Placement.h"
#include "Cell.h"
#include "Site.h"

using namespace std;


Placement::Placement (int rows, int cols, 
		vector<vector<Site>>& inputSitemap) {
	_rows = rows;
	_cols = cols;
	_placement.swap(inputSitemap);	
}

void Placement::addCells (const unordered_map<string, Cell>::const_iterator& begin, const unordered_map<string, Cell>::const_iterator& end) {	
	for ( auto it = begin; it != end; ++it) {
		string newCellName = (it->first);
		Cell newCell = (it->second);
		int xCell = newCell.getX();
		int yCell = newCell.getY();

		(_placement[yCell][xCell]).addCell( newCellName );
	}
}


vector<Site> Placement::getRow (int row) {
	return _placement[row];
}


Site Placement::getSite (int row, int col) {
	return _placement[row][col];
}

int Placement::getRows () {
	return _rows;
}

int Placement::getCols () {
	return _cols;
}
