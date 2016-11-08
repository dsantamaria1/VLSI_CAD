
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <utility>

using namespace std;

void print(const string *s) {
    if (s == nullptr) {
        cout << "nullptr!" << endl;
    }
    else {
        cout << *s << endl;
    }
}

int main() {
   
    vector<unique_ptr<string>> v(10);
    cout << v.size() << endl;

    v.at(0) = make_unique<string>("hello world!");
    cout << *v.at(0) << endl;
    print(v.at(1).get());


    v.at(1) = move(v.at(0));
    print(v.at(0).get());
    print(v.at(1).get());

    return 0;
}
