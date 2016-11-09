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

void Placement::addCells (int x, int y, unordered_map<string, Cell>* cellMap) {
	
}


vector<Site> Placement::getRow (int row) {
	return _placement[row];
}


Site Placement::getSite (int row, int col) {
	return _placement[row][col];
}

