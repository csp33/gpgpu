#include "Common.h"
#include "cl.hpp"
#include "tools.h"
//#include <CL/cl.hpp>
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;

#define PRINT_TIME 0

vector<int> dijkstra_parallel(const vector<vector<int>> &adjacency) {

  vector<int> distances; // Distances from the first element to the i'st
  vector<int> visited;   // True if the vertex i has been visited.
  int size = adjacency.capacity();
  distances.resize(size);
  visited.resize(size);
  // As OpenCL does not support matrices, we must transform it into a vector.
  vector<int> adj_vector = get_vector_from_matrix(adjacency);

  // Initialize the distances as +INF and visited to false:
  for (int i = 0; i < size; i++) {
    visited[i] = false;
    distances[i] = INT_MAX;
  }

  distances[0] = 0; // Distance of source to itself is 0

  /**** OpenCL find_minimum creation *****/

  cl_int err = CL_SUCCESS;

  // Get a platform ID

  vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  if (platforms.empty()) {
    cerr << "Unable to find suitable platform." << endl;
    exit(-1);
  }

  // Create a context

  cl_context_properties properties[] = {
      CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};
  cl::Context context(CL_DEVICE_TYPE_GPU, properties);

  // Enumerate the devices

  vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

  // Create the command queue

  cl::CommandQueue queue(context, devices[0], 0, &err);

  // Create the OpenCL program

  string programSource = FileToString("./src/programs.cl");
  cl::Program program = cl::Program(context, programSource);
  program.build(devices);

  // Create the buffers

  cl::Buffer cl_visited =
      cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * size, NULL, &err);
  cl::Buffer cl_distances =
      cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * size, NULL, &err);

  cl::Buffer cl_adjacency =
      cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(int) * adj_vector.capacity(),
                 NULL, &err);
  cl::Buffer cl_minimums =
      cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * 1, NULL, &err);

  // Copy the content to the buffers

  queue.enqueueWriteBuffer(cl_visited, true, 0, sizeof(int) * size,
                           visited.data());
  queue.enqueueWriteBuffer(cl_distances, true, 0, sizeof(int) * size,
                           distances.data());

  queue.enqueueWriteBuffer(cl_adjacency, true, 0,
                           sizeof(int) * adj_vector.capacity(),
                           adj_vector.data());

  cl::Event event;

  /** First part: get the minimum    **/

  cl::Kernel find_minimum(program, "min_distance", &err);
  CheckCLError(err);

  /** Second part: update neighbours **/

  cl::Kernel update_neighbors(program, "dijkstra", &err);
  CheckCLError(err);

  // Set the find_minimum parameters

  find_minimum.setArg(0, cl_visited);
  find_minimum.setArg(1, cl_distances);
  find_minimum.setArg(2, cl_minimums);
  find_minimum.setArg(3, size);

  update_neighbors.setArg(0, cl_visited);
  update_neighbors.setArg(1, cl_adjacency);
  update_neighbors.setArg(2, cl_distances);
  update_neighbors.setArg(3, size);
  update_neighbors.setArg(4, cl_minimums);

  for (int i = 0; i < size - 1; i++) {

    queue.enqueueNDRangeKernel(find_minimum, cl::NullRange, cl::NDRange(1, 1),
                               cl::NullRange, NULL, &event);

    event.wait();

    queue.enqueueNDRangeKernel(update_neighbors, cl::NullRange,
                               cl::NDRange(size, 1), cl::NullRange, NULL,
                               &event);

    event.wait();
  }
  queue.enqueueReadBuffer(cl_distances, true, 0, sizeof(int) * size,
                          distances.data());

  return distances;
}

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
  distances = dijkstra_parallel(adjacency);
  end = clock();
  double secs = double(end - begin) / CLOCKS_PER_SEC;

#if PRINT_TIME
  cout << adjacency.capacity() << " " << secs << endl;
#else
  cout << distances << endl;
#endif

}
