#ifndef TOOLS
#define TOOLS
#include <vector>
#include <fstream>
#include <climits>
#include <cassert>

using namespace std;

vector<int> get_vector_from_matrix(const vector<vector<int>> &matrix);
vector<vector<int>> get_adjacency_matrix(const string &name);
ostream &operator<<(ostream &os, const vector<vector<int>> &matrix);
ostream &operator<<(ostream &os, const vector<int> &v);
void write_to_file(string name, const vector<vector<int>> &matrix) ;
vector<int> dijkstra_sequential(const vector<vector<int>> &adjacency);
int min_distance(const vector<int> &distances, const vector<bool> &visited);
#endif
