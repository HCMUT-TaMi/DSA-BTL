#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

// #include <filesystem> //require C++17
// namespace fs = std::filesystem;

// #include "hash/xMap.h"
#include "list/listheader.h"
// #include "sformat/fmt_lib.h"
// #include "tensor/xtensor_lib.h"
// #include "ann/annheader.h"
// #include "loader/dataset.h"
// #include "loader/dataloader.h"
// #include "config/Config.h"
// #include "dataset/DSFactory.h"
// #include "optim/Adagrad.h"
// #include "optim/Adam.h"
// #include "modelzoo/twoclasses.h"
// #include "modelzoo/threeclasses.h"
// #include "tensor/xtensor_lib.h"
#include "graph/DGraphModel.h"
#include "graph/AbstractGraph.h"
#include "graph/UGraphModel.h"
// #include "demo/graph/DGraphDemo.h"
#include "sorting/DLinkedListSE.h"
#include "graph/TopoSorter.h"

bool charComparator(char& lhs, char& rhs){
    return lhs==rhs;
}

int intKeyHash(char &key, int capacity) { return key % capacity; }


string vertex2str(char& v){
    stringstream os;
    os << v;
    return os.str();
}


void Sorttest()
{
    DLinkedList<int> un_order; 
    for(int i = 20; i >= 0; i--) 
    {
      un_order.add(i);
    }
  

    cout << "before Sorting: \n"; 
    cout << un_order.toString();
    DLinkedListSE<int> order(un_order);


    cout << "\nafter Sorting: \n";  
    order.sort(); 
    un_order = order; 
    cout << un_order.toString() << un_order.size();
    cout << order.toString(); 
}

int intKeyHash(int& key, int tablesize) {
    return key % tablesize;
}

// void test()
// {
//    DGraphModel<int> model(&charComparator, &vertex2str);
//     for (int idx = 0; idx < 7; idx++)
//     {
//         model.add(idx);
//     }
//     model.connect(0, 1);
//     model.connect(1, 3);
//     model.connect(1, 2);
//     model.connect(1, 4);
//     model.connect(2, 5);
//     model.connect(5, 6);
//     model.connect(4, 6);

//     TopoSorter<int> topoSorter(&model, &intKeyHash);
//     DLinkedList<int> result = topoSorter.sort(TopoSorter<int>::DFS,false);
//     cout << endl << "MY RESULT: " << result.toString(); 


// }

void newTest()
{
  string name = "graph23";
  //! data ------------------------------------
  UGraphModel<char> model(&charComparator, &vertex2str);
  char vertices[] = {'A', 'B', 'C', 'D'};
  for (int idx = 0; idx < 4; idx++) {
    model.add(vertices[idx]);
  }
  model.connect('A', 'B', 5);

  cout << model.toString(); 

  //! expect ----------------------------------
  string expect =
      "5\n\
Error: Vertex (X): is not found\n\
Error: Vertex (Y): is not found\n\
Error: Edge (E(A,C)): is not found\n\
==================================================\n\
Vertices:   \n\
V(A, in: 1, out: 1)\n\
V(B, in: 1, out: 1)\n\
V(C, in: 0, out: 0)\n\
V(D, in: 0, out: 0)\n\
------------------------------\n\
Edges:      \n\
E(A,B,5)\n\
E(B,A,5)\n\
==================================================\n";

  //! output ----------------------------------
  stringstream output;
  output << model.weight('A', 'B') << endl;

  try {
    model.weight('A', 'X');
  } catch (const exception& e) {
    cout << "Error: " << string(e.what()) << endl;
  }

  try {
    model.weight('Y', 'A');
  } catch (const exception& e) {
    cout << "Error: " << string(e.what()) << endl;
  }

  cout << "good"; 
  try {
    model.weight('A', 'C');
  } catch (const exception& e) {
    cout << "Error: " << string(e.what()) << endl;
  }

  //! remove data -----------------------------
  model.clear();
  cout << "Clear here" << endl;  
  cout << model.toString(); 
}

int compare(int& lhs, int& rhs) {
    if (lhs > rhs) return -1; // lhs lớn hơn rhs -> giữ nguyên thứ tự
    else if (lhs < rhs) return 1; // lhs nhỏ hơn rhs -> đảo thứ tự
    else return 0; // Bằng nhau
}

void failTest()
{
    DGraphModel<char> model(&charComparator, &vertex2str);

    char vertices[] = {'F','G','B', 'C', 'D', 'E', 'A'};
    for (int idx = 0; idx < 7; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('B', 'D');
    model.connect('B', 'C');
    model.connect('B', 'E');
    model.connect('C', 'G');
    model.connect('G', 'F');
    model.connect('E', 'F');

    TopoSorter<char> topoSorter(&model, &intKeyHash);
    DLinkedList<char> result = topoSorter.sort(TopoSorter<char>::DFS);

    //! output ----------------------------------
    cout << "DFS Topological Sort: ";

    for (auto it = result.begin(); it != result.end(); it++)
    {
        cout << *it << "->";
    }
}

int hashFunc(char& key, int tablesize) {
    return key % tablesize;
}
void DGraphDemo3()
{
   DGraphModel<char> model(&charComparator, &vertex2str);
    for (int idx = 0; idx < 10; idx++)
    {
        model.add((char)('0' + idx));
    }

    model.connect('0', '1');
    model.connect('0', '5');
    model.connect('1', '7');
    model.connect('3', '2');
    model.connect('3', '4');
    model.connect('3', '7');
    model.connect('3', '8');
    model.connect('4', '8');
    model.connect('6', '0');
    model.connect('6', '1');
    model.connect('6', '2');
    model.connect('8', '2');
    model.connect('8', '7');
    model.connect('9', '4');
    model.println();

    TopoSorter<char> sorter(&model, &hashFunc);
    DLinkedList<char> bfs = sorter.sort(TopoSorter<char>::BFS);
    cout << left << setw(15) << "Topo-order (BFS): " << bfs.toString() << endl;

    DLinkedList<char> dfs = sorter.sort(TopoSorter<char>::DFS);
    cout << left << setw(15) << "Topo-order (DFS): " << dfs.toString() << endl;
}

void DGraphDemo2()
{
    DGraphModel<char> model(&charComparator, &vertex2str);
    char vertices[] = {'A', 'B', 'C', 'D'};
    for (int idx = 0; idx < 4; idx++)
    {
        model.add(vertices[idx]);
    }
    model.connect('A', 'B');
    model.connect('B', 'D');
    model.connect('C', 'B');
    model.connect('C', 'D');
    model.println();

    TopoSorter<char> sorter(&model, &hashFunc);
    DLinkedList<char> topo = sorter.sort(TopoSorter<char>::BFS);
    cout << topo.toString(); 

}

int main(int argc, char** argv) {
 
    DGraphDemo3(); 
}
