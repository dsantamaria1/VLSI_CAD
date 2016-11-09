#ifndef NET_H
#define NET_H

#include <string>
#include <vector>


using namespace std;

class Net {
public:
	Net (string name, vector<string> cells);
	Net (const Net& net);
private:
	string _name;
	vector<string> _cells;	
};

#endif
