#include "Parser.h"
#include "Cell.h"
#include "Placement.h"
#include "Net.h"
#include "Site.h"
#include <iostream>
#include <vector>
using namespace std;

int main (int argc, char* argv[]) {
    string basePath = argv[1];
    string benchmark = argv[2];
    Parser p = Parser(basePath, benchmark);
    unordered_map<string, Cell> placement = p.parsePlacement();
    unordered_map<string, Net> netlist = p.parseNetlist();
    vector<vector<Site>> sitemap = p.parseSitemap();
    vector< vector<Site> >::iterator row;
    vector<Site>::iterator col;
    vector<string> __cellNames;
    Cell c;
    // for (row = sitemap.begin(); row != sitemap.end(); row++) {
    //     for (col = row->begin(); col != row->end(); col++) {
    //        cout << col->get_type() << endl;
    //     }
    // }


    for ( auto it = netlist.begin(); it != netlist.end(); ++it ){
       //cout << " " << it->second  << endl;
       Net n = it->second;
       __cellNames = n.getCellNames();
       for(int i=0; i < __cellNames.size(); i++)
        c = placement[__cellNames[i]];
        cout << "Name: " << c.getName() << " Type: " << c.getType()<<endl;

    }
}
