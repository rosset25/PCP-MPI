# include <stdio.h>
# include "mpi.h"

int main ( int argc , char * argv [] ) {
 
  int numProcs , miId ;
  int n;
  
  MPI_Init ( & argc , & argv ) ;

  MPI_Comm_size ( MPI_COMM_WORLD , & numProcs ) ;
  MPI_Comm_rank ( MPI_COMM_WORLD , & miId ) ;

  if ( miId == 0) {
    printf( "Dame un numero: ");
    scanf("%f", &n);
    
  }
  
  printf("Soy el proceso %d, n = %d \n", miId, n);
  //a n no se le ha asignado el valor leído por pantalla al resto de procesos

  MPI_Finalize () ;
  printf ( " Fin de programa \n " ) ;
  return 0;
}
 