#include <string>
#include <vector>
#include <unordered_map>

#include "Cell.h"
#include "Site.h"

using namespace std;


class Placement {
public:
	Placement (int rows, int cols, vector<vector<Site>>& inputSitemap);
	void addCells (const unordered_map<string, Cell>::const_iterator& begin, const unordered_map<string, Cell>::const_iterator& end);
	void swapCells (int row1, int col1, int row2, int col2);
	vector<Site> getRow (int row);
	Site getSite (int row, int col);
	int getRows ();
	int getCols ();
	void checkValidity ( unordered_map<string, Cell>* cellMap ); 
private:
	int _rows;
	int _cols;
	vector<vector<Site>> _placement;
};
