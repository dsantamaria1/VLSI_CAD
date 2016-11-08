#include <string>
#include <vector>
#include <unordered_map>

#include "Cell.h"
#include "Site.h"

using namespace std;


class Placement {
public:
	Placement (int rows, int cols, vector<vector<Site>>& inputSitemap);
	void addCells (int x, int y, unordered_map<string, Cell>* cellMap);
	vector<Site> getRow (int row);
	Site getSite (int row, int col);
private:
	int _rows;
	int _cols;
	vector<vector<Site>> _placement;
};
