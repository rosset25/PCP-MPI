#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "math.h"



int main( int argc, char *argv[] ) {

  int numProcs, miId, dato, suma, temp;
   
  
  // Inicializa MPI.
  MPI_Init( & argc, & argv );
  MPI_Comm_size( MPI_COMM_WORLD, & numProcs );
  MPI_Comm_rank( MPI_COMM_WORLD, & miId );

  dato = numProcs - miId +1;

  printf("Soy el proceso: %d, y mi dato es: %d \n", miId, dato);
   
  suma = 0;
  
  if( miId%2 == 0){ //procesos pares se quedan su dato para la suma
    temp = dato;
  }else{
    temp = 0; //procesos impares participarán en la suma con 0
  }
  
  //suma colectiva
  MPI_Reduce(& temp, &suma, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  
  
  if (miId == 0){
     printf("El total de la suma es: %d \n", suma);
  }
 
 
 
  MPI_Finalize();

  if(miId == 0) {
    printf("Fin de programa");
  }
  
  return 0;
  
}