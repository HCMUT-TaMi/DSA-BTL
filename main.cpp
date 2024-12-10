#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "include/list/listheader.h"
#include "include/graph/DGraphModel.h"
#include "include/graph/AbstractGraph.h"

using namespace std;
bool charComparator(int& lhs, int& rhs){
    return lhs==rhs;
}
string vertex2str(int& v){
    stringstream os;
    os << v;
    return os.str();
}

void test()
{
    DGraphModel<int> t(&charComparator,&vertex2str); 
    t.add(1); 
    t.add(2); 
    t.add(3); 
    cout << "HELLO"; 
}


int main(int argc, char** argv) {
 
    test(); 
    return 0;
}
