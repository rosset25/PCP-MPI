#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "math.h"



int main( int argc, char *argv[] ) {

   int numProcs, miId, dato, suma, i;
   MPI_Status s;
  
  // Inicializa MPI.
  MPI_Init( & argc, & argv );
  MPI_Comm_size( MPI_COMM_WORLD, & numProcs );
  MPI_Comm_rank( MPI_COMM_WORLD, & miId );

  dato = numProcs - miId +1;
 
  if (miId == 0){
    
    suma = dato;
    
    printf("Soy el proceso: %d, y mi dato es: %d \n", miId, dato);
    
    for(i =1; i < numProcs; i++){
       MPI_Recv(& dato, 1, MPI_INT, MPI_ANY_SOURCE, 88, MPI_COMM_WORLD, &s ); //proceso 0 recibe el dato del resto
       suma += dato;
    }
    
    printf("El total de la suma es: %d\n", suma);
    
  }else{
    
    MPI_Send(& dato, 1, MPI_INT, 0, 88, MPI_COMM_WORLD );
    printf("Soy el proceso: %d, y mi dato es: %d \n", miId, dato);
  }
 
 
 
 
 MPI_Finalize();

 printf("Fin de programa");
  
  return 0;
}