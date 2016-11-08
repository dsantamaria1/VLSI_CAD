#include <string>
#include <vector>
#include "Site.h"

using namespace std

class Sitemap {
public:
	Sitemap (int _rows, int _cols, const vector<vector<Site>>& _sitemap);
	vector<Site> get_row(int row);
	Site get_site(int row, int col);
	void check_legality(Placement pl);
private:
	const int rows;
	const int cols;
	vector<vector<Site>> sitemap;
};
