
__kernel void min_distance(__global int* visited, __global int* distances,
                           __global int* minimums,const int size){

        int min = INFINITY;
        for(int k=0; k<size; k++) {
                if(!visited[k] && distances[k] <= min) {
                        min = distances[k];
                        *minimums = k;
                }
        }
}

__kernel void dijkstra(__global int* visited,__global int* adjacency,
                       __global int* distances,const int size,__global int *indexes){
        visited[*indexes]=1;
        int j = get_local_id(0);
        if (!visited[j] && distances[*indexes] + adjacency[j + *indexes*size] < distances[j])
                distances[j] = distances[*indexes] + adjacency[j + *indexes*size];
}

/*
   __kernel void min_distance(__global int* visited, __global int* distances,
                              __global int* minimums, __global int *indexes){
       int id = get_global_id(0);
       minimums[id]=distances[id];
       barrier(CLK_GLOBAL_MEM_FENCE);
       for(unsigned int s = get_global_size(0)/2; s>0; s>>=1) {
               if(id<s && !visited[s]){
                 if(minimums[id]<minimums[id+s]){
                   minimums[id]=minimums[id];
                   indexes[id]=id;
                 }
                 else{
                   minimums[id]=minimums[id+s];
                   indexes[id]=id+s;
                 }
               }
            //           minimums[id]=min(distances[id],distances[id+s]);
               barrier(CLK_GLOBAL_MEM_FENCE);
       }
   }
*/

/*
   __kernel void min_distance(__global int* visited, __global int* distances,__global int* minimums){
        int id = get_global_id(0);
        for(unsigned int s = get_global_size(0)/2; s>0; s>>=1) {
                if(id<s && !visited[s])
                        minimums[id]=min(distances[id],distances[id+s]);
                barrier(CLK_GLOBAL_MEM_FENCE);
        }
   }
 */

/*
   __kernel void dijkstra(__global int* visited,__global int* adjacency,
                       __global int* distances,const int size,__global int* minimums){
        visited[*minimums]=1;
        int j = get_local_id(0);
        if (!visited[j] && distances[*minimums] + adjacency[j + *minimums*size] < distances[j])
                distances[j] = distances[*minimums] + adjacency[j + *minimums*size];

   }
 */

/*
   __kernel void min_distance(__global int* visited, __global int* distances,__global int* minimums){

        int min = INFINITY;
        int k = get_local_id(0);
        if(!visited[k] && distances[k] <= min) {
                min = distances[k];
 * minimums = k;
        }
 */

/**
   __kernel void dijkstra(__global int* visited,__global int* adjacency,
                       __global int* distances,const int size){
        for (int i = 0; i < size; i++) {
                //Find the vertex with the smallest distance
                int min = INFINITY;
                int min_idx;
                for(int k=0; k<size; k++) {
                        if(!visited[k] && distances[k] <= min) {
                                min = distances[k];
                                min_idx = k;
                        }
                }

                barrier(CLK_GLOBAL_MEM_FENCE);

                // Update all its neighbours
                int u=min_idx;
                visited[u]=1;
                int j = get_local_id(0);
                if (!visited[j] && distances[u] + adjacency[j + u*size] < distances[j])
                        distances[j] = distances[u] + adjacency[j + u*size];
        }

   }

 **/

// Reduction to find the minimum (2 kernels)
