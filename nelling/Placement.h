#include <string>
#include <vector>
#include <memory>

#include "Cell.h"
#include "Site.h"

using namespace std;


class Placement {
public:
	Placement (int rows, int cols, vector<vector<Site>>& inputSitemap,
		vector<vector<Cell>>& inputPlacement);
	vector<Site*> getSite (int row);
	Site* getSite (int row, int col);
private:
	int _rows;
	int _cols;
	vector<vector<Site>> _placement;
};
