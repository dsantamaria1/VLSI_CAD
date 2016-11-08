#include <string>
#include <vector>
#include "Sitemap.h"

using namespace std;


Sitemap::Sitemap (int _rows, int _cols, const vector<vector<Site>>& _sitemap) :
	rows(_rows),
	cols(_cols),
	sitemap(_sitemap)
{}

vector<Site> Sitemap::get_row(int row) {
	return nullptr;
}

Site Sitemap::get_site(int row, int col) {
	return nullptr;
}

Sitemap::check_legality(Placement pl) {

}
