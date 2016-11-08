#include <string>
#include <vector>
#include "Net.h"

using namespace std

Net::Net (const string& _name, const vector<string>& _cells) :
	name (_name),
	cells (_cells)
{}
