#include <string>
#include <vector>
#include <memory>
#include <utility>

#include "Placement.h"
#include "Cell.h"
#include "Site.h"

using namespace std;


Placement::Placement (int rows, int cols, 
		vector<vector<Site>>& inputSitemap,
		vector<vector<Cell>>& inputPlacement) {
	_rows = rows;
	_cols = cols;
	_placement.swap(inputSitemap);
	
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			_placement[r][c].addCell(inputPlacement[r][c]);
		}
	}
}

vector<Site*> Placement::getRow (int row) {

}

Site* Placement::getSite (int row, int col) {

}


int main () {}
