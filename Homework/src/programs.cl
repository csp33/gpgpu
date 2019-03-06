__kernel void dijkstra(__global int* visited,__global int* adjacency,__global int* distances,const int size){

   int min = 999999999;
   for (int i = get_group_id(0); i < size; i++) {
    int min_idx;
    for(int k=0;k<size;k++){
      if(!visited[k] && distances[k] <= min){
        min = distances[k];
        min_idx = k;
      }
    }

    int u=min_idx;

    for (int j = get_local_id(0); j < size; j++) {
      if (!visited[j] && distances[u] + adjacency[j + u*size] < distances[j])
        distances[j] = distances[u] + adjacency[j + u*size];
    }
   }

}
