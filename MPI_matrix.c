#include <stdio.h>                                                                                                                        
#include <stdlib.h>                                                                                                                       
#include <mpi.h>                                                                                                                          
                                                                                                                                         
int main(int argc, char** argv) {                                                                                                         
   int rank, size;                                                                                                                       
   MPI_Init(&argc, &argv);                                                                                                               
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);                                                                                                 
   MPI_Comm_size(MPI_COMM_WORLD, &size);                                                                                                 
                                                                                                                                         
   int m = 10; // count of rows                                                                                                          
   int n = 20; // count for columns                                                                                                      
   int r = 2;  // count of rows per process                                                                                              
                                                                                                                                         
   // matrix initialize                                                                                                                  
   double* matrix = malloc(sizeof(double) * m * n);                                                                                      
   int i;                                                                                                                                
   for (i = 0; i < m * n; i++) {                                                                                                         
       matrix[i] = (double) rand() / RAND_MAX;                                                                                           
   }                                                                                                                                     
                                                                                                                                         
   // divide matrix per process                                                                                                          
   int count = r * n;                                                                                                                    
   double* local_matrix = malloc(sizeof(double) * count);                                                                                
   MPI_Scatter(matrix, count, MPI_DOUBLE, local_matrix, count, MPI_DOUBLE, 0, MPI_COMM_WORLD);                                           
                                                                                                                                         
   // search max elemeent per part                                                                                                       
   double local_max = local_matrix[0];                                                                                                   
   for (i = 0; i < r * n; i++) {                                                                                                         
       if (local_matrix[i] > local_max) {                                                                                                
           local_max = local_matrix[i];                                                                                                  
       }                                                                                                                                 
   }                                                                                                                                     
                                                                                                                                         
   // max element global                                                                                                                 
   double global_max;                                                                                                                    
   MPI_Reduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);                                                       
                                                                                                                                         
   if (rank == 0) {                                                                                                                      
       // print generated matrix                                                                                                         
       printf("Initialized matrix: \n");                                                                                                 
       for (i = 0; i < m * n; i++) {                                                                                                     
               printf("%f ", matrix[i]);                                                                                                 
               if ((i + 1) % n == 0) {                                                                                                   
                       printf("\n");                                                                                                     
               }                                                                                                                         
      }                                                                                                                                  
       // print global max element                                                                                                       
       printf("Maximum element: %f\n", global_max);                                                                                      
   }                                                                                                                                     
                                                                                                                                         
   free(matrix);                                                                                                                         
   free(local_matrix);                                                                                                                   
   MPI_Finalize();                                                                                                                       
   return 0;                                                                                                                             
}
