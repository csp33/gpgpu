#include <fstream>
#include <iostream>
#include "tools.h"
#include <stdlib.h>
#include <vector>
using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "ARGUMENTS ERROR! " << argv[0] << " number of nodes" << endl;
    exit(-1);
  }
  srand(time(NULL));

  int size = atoi(argv[1]);
  vector<vector<int>> adjacency;

  adjacency.reserve(size);
  for (int i = 0; i < size; i++)
    adjacency[i].reserve(size);

  for (int i = 0; i < size; i++) {
    for (int j = 0; j <= i; j++) {
      if (i == j)
        adjacency[i][j] = 0;
      else {
        int distance;
        do {
          distance = rand() % 10;
        } while (distance == 0);
        adjacency[i][j] = distance;
        adjacency[j][i] = distance;
      }
    }
  }
  string name = "graph" + to_string(size) + ".txt";
  write_to_file(name,adjacency);

  return 0;



}
