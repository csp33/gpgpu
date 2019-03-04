#include "tools.cpp"
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;

#define PRINT_TIME 1

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "ERROR! " << argv[0] << " graph.txt" << endl;
    exit(-1);
  }
  string file = argv[1];
  vector<vector<int>> adjacency;
  adjacency = get_adjacency_matrix(file);
  vector<int> distances;
  clock_t begin, end;
  begin = clock();
  distances = dijkstra_sequential(adjacency);
  end = clock();
  double secs = double(end - begin) / CLOCKS_PER_SEC;

#if PRINT_TIME
  cout << adjacency.capacity() << " " << secs << endl;
#else
  cout<<distances<<endl;
#endif
}
