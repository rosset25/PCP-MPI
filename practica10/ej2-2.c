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
    
    MPI_Recv(& dato, 1, MPI_INT, 1, 88, MPI_COMM_WORLD, &s ); //proceso 0 recibe el dato del resto
    suma += dato;
    
    printf("El total de la suma es: %d\n", suma);
    
  }else{
    
    if(miId%2 == 0){  //envian solo los procesos pares
      MPI_Send(& dato, 1, MPI_INT, 1, 88, MPI_COMM_WORLD );
    }
    
    if(miId == 1){  //el proceso 1 recoge los datos del resto de procesos menos el 0
      suma = 0;
      dato = 0;
      for(i =2; i < numProcs; i+=2){
	MPI_Recv(& dato, 1, MPI_INT, MPI_ANY_SOURCE, 88, MPI_COMM_WORLD, &s ); //proceso 0 recibe el dato del resto
	suma += dato;
      }
      
      MPI_Send(& suma, 1, MPI_INT, 0, 88, MPI_COMM_WORLD );
      
    }
    
    //imprimen todos los procesos (pares e impares)
    printf("Soy el proceso: %d, y mi dato es: %d \n", miId, dato);
    
  }
  
   
 
  MPI_Finalize();

  if (miId == 0){
    printf("Fin de programa");
  }
  
  
  return 0;
}