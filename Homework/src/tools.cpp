
#include "tools.h"
#include <iostream>

vector<int> get_vector_from_matrix(const vector<vector<int>> &matrix) {
  vector<int> result;
  for (int i = 0; i < matrix.capacity(); i++)
    for (int j = 0; j < matrix.capacity(); j++)
      result.push_back(matrix[i][j]);
  return result;
}

vector<vector<int>> get_adjacency_matrix(const string &name) {
  ifstream file;
  int size;
  string tmp;
  file.open(name);
  assert(file);
  file >> tmp;  // read the word "SIZE"
  file >> size; // Get the size
  vector<vector<int>> adjacency;
  adjacency.resize(size);
  for (int i = 0; i < size; i++)
    adjacency[i].resize(size);

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      file >> adjacency[i][j];
  return adjacency;
}

ostream &operator<<(ostream &os, const vector<vector<int>> &matrix) {
  for (int i = 0; i < matrix.capacity(); i++) {
    for (int j = 0; j < matrix.capacity(); j++)
      os << matrix[i][j] << " ";
    os << endl;
  }
  return os;
}

ostream &operator<<(ostream &os, const vector<int> &v) {
  for (int i = 0; i < v.capacity(); i++)
    os << v[i] << " ";
  return os;
}

void write_to_file(string name, const vector<vector<int>> &matrix) {
  ofstream file;
  file.open(name);
  file << "SIZE " << matrix.capacity() << endl;
  for (int i = 0; i < matrix.capacity(); i++) {
    for (int j = 0; j < matrix.capacity(); j++)
      file << matrix[i][j] << " ";
    file << endl;
  }
  file.close();
}

int min_distance(const vector<int> &distances, const vector<int> &visited) {
  int min = INT_MAX;
  int min_idx;

  for (int i = 0; i < distances.capacity(); i++) {
    if (!visited[i] && distances[i] <= min) {
      min = distances[i];
      min_idx = i;
    }
  }
  return min_idx;
}

int min_distance(const vector<int> &distances, const vector<bool> &visited) {
  int min = INT_MAX;
  int min_idx;

  for (int i = 0; i < distances.capacity(); i++) {
    if (!visited[i] && distances[i] <= min) {
      min = distances[i];
      min_idx = i;
    }
  }
  return min_idx;
}

vector<int> dijkstra_sequential(const vector<vector<int>> &adjacency) {
  vector<int> distances; // Distances from the first element to the i'st
  vector<bool> visited;  // True if the vertex i has been visited.
  int size = adjacency.capacity();
  distances.resize(size);
  visited.resize(size);

  // Initialize the distances as +INF and visited to false:
  for (int i = 0; i < size; i++) {
    visited[i] = false;
    distances[i] = INT_MAX;
  }

  distances[0] = 0; // Distance of source to itself is 0

  for (int i = 0; i < size - 1; i++) {
    int u = min_distance(distances, visited);
    visited[u] = true;
    for (int j = 0; j < size; j++) {
      if (!visited[j] && distances[u] + adjacency[u][j] < distances[j])
        distances[j] = distances[u] + adjacency[u][j];
    }
  }
  return distances;
}
