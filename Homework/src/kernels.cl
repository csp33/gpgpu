__kernel void dijkstra(_global int **adjacency, _global bool *visited,
                      _global *int distances, const int size) {

  for (int i = get_group_id(0); i < size; i++) {
    int min = 999999999;
    int min_idx;
    for(int k=0;k<size;k++){
      if(!visited[k] && distances[k] <= min){
        min = distances[k];
        min_idx = k;
      }
    }

    int u=min_idx;

    // Calcular min_distance

    for (int j = get_local_id(0); j < size; j++) {
      if (!visited[j] && distances[u] + adjacency[u][j] < distances[j])
        distances[j] = distances[u] + adjacency[u][j];
    }
  }
}
