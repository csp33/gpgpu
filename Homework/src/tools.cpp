
#include "tools.h"
#include <iostream>

vector<vector<int>> get_adjacency_matrix(const string &name) {
  ifstream file;
  int size;
  string tmp;
  file.open(name);
  assert(file);
  file >> tmp;  // read the word "SIZE"
  file >> size; // Get the size
  vector<vector<int>> adjacency;
  adjacency.reserve(size);
  for (int i = 0; i < size; i++)
    adjacency[i].reserve(size);

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
  distances.reserve(size);
  visited.reserve(size);

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

vector<int> dijkstra_parallel(const vector<vector<int>> &adjacency) {

  vector<int> distances; // Distances from the first element to the i'st
  vector<bool> visited;  // True if the vertex i has been visited.
  int size = adjacency.capacity();
  distances.reserve(size);
  visited.reserve(size);

  // Initialize the distances as +INF and visited to false:
  for (int i = 0; i < size; i++) {
    visited[i] = false;
    distances[i] = INT_MAX;
  }

  distances[0] = 0; // Distance of source to itself is 0

  cl_int err = CL_SUCCESS;

  // Get a platform ID
  vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  if (platforms.size() == 0) {
    cout << "Unable to find suitable platform." << endl;
    exit(-1);
  }

  // Create a context
  cl_context_properties properties[] = {
      CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};
  cl::Context context(CL_DEVICE_TYPE_GPU, properties);

  // Enumerate the devices
  vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

  // Create the command queue
  cl::Event event;
  cl::CommandQueue queue(context, devices[0], 0, &err);

  // Create the OpenCL program
  string programSource = FileToString("./kernels.cl");
  cl::Program program = cl::Program(context, programSource);
  program.build(devices);

  // Get the kernel handle
  cl::Kernel kernel(program, "dijkstra", &err);
  CheckCLError(err);

  // Create the buffers

  cl::Buffer cl_adjacency = cl::Buffer(
      context, CL_MEM_READ_ONLY,
      sizeof(int) * adjacency.capacity() * adjacency.capacity(), NULL, &err);
  cl::Buffer cl_visited =
      cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(bool) * visited.capacity(),
                 NULL, &err);
  cl::Buffer cl_distances =
      cl::Buffer(context, CL_MEM_READ_WRITE,
                 sizeof(bool) * distances.capacity(), NULL, &err);

  // Copy the content

  queue.enqueueWriteBuffer(cl_adjacency, true, 0,
                           sizeof(int) * adjacency.capacity(),
                           adjacency.data());
  queue.enqueueWriteBuffer(cl_visited, true, 0,
                           sizeof(bool) * visited.capacity(), visited.data());
  queue.enqueueWriteBuffer(cl_distances, true, 0,
                           sizeof(int) * distances.capacity(),
                           distances.data());
  // Set the kernel parameters
  kernel.setArg(0, cl_adjacency);
  kernel.setArg(1, cl_visited);
  kernel.setArg(2, cl_distances);
  kernel.setArg(3, distances.capacity());

  queue.enqueueNDRangeKernel(kernel, cl::NullRange,
                             cl::NDRange(distances.capacity(), 1),
                             cl::NullRange, NULL, &event);
  event.wait();
  queue.enqueueReadBuffer(cl_distances, true, 0,
                          sizeof(int) * distances.capacity(), distances.data());

  return distances;
}
