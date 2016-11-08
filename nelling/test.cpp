#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;


int main () {
	string cat = "cat";
	string hat = "hat";
	const string *hatptr = &hat;	

	cout << "hat: " << hat << endl;
	cout << "hatptr: " << hatptr << "->" << *hatptr << endl;

	//*hatptr = "not hat";
	hat = "not hat";
	
	cout << "hatptr: " << hatptr << "->" << *hatptr <<  endl;
}
