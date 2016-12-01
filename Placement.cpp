#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>

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

void Placement::swapCells (int row1, int col1, int row2, int col2) {
	string tempName = _placement[row1][col1].getCellName();
	_placement[row1][col1].addCell( _placement[row2][col2].getCellName() );
	_placement[row2][col2].addCell( tempName );
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

void Placement::checkValidity ( unordered_map<string, Cell>* cellMap ) {
	
	int cellCount = (*cellMap).size();
	int mismatchCount = 0;

	for (int r = 0; r < _rows; r++) {
		for (int c = 0; c < _cols; c++) {
			Site site = _placement[r][c];
			
			if (site.getCellName() != "") {
				Cell cell = (*cellMap)[ site.getCellName() ];
				cellCount--;
				
				if (cell.getX() != c || cell.getY() != r) {
					cout << "Warning: Cell Map and Placement disagree for ";
					cout << cell.getName() << "\n";
				}
	
				if ( site.getType() != cell.getType() ) {
					cout << "Cell " << cell.getName() << "is misplaced.\n";
					cout << cell.getType() << " does not match " << site.getType();
					cout << "\n";
					mismatchCount++;
				}
			}
		}
	}


	cout << "Mismatch Count: " << mismatchCount << "\n";
	
	if (cellCount > 0) {
		cout << "Warning: " << cellCount << " cells missing from placement.\n";
	}

}



void Placement::printPlacement ( string filename, 
		unordered_map<string, Cell>* cellMap) {
	ofstream outfile;

	try {
		outfile.open(filename);
		
		for (int r = 0; r < _rows; r++) {
			for (int c = 0; c < _cols; c++) {
				string cellName = _placement[r][c].getCellName();
				if ( cellName.empty() ) { continue; }
				
				Cell cell = (*cellMap)[ cellName ];

				outfile << cell.getName() << " ";
				outfile << cell.getType() << " ";
				outfile << cell.getX() << " ";
				outfile << cell.getY() << " ";
				
				if ( cell.isFixed() ) {
					outfile << "F" << endl; 
				} else {
					outfile << "M" << endl;
				}
			}
		}	
		outfile.close();	
	}
	catch (std::ofstream::failure e) {
		cerr << "Exception occured during file handling." << endl;
	}	

}
