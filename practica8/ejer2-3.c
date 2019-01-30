# include <stdio.h>
# include "mpi.h"

int main ( int argc , char * argv [] ) {
 
  int numProcs , miId ;
  int n, i; //n lo hemos tratado como entero, podríamos haber sido un float
  MPI_Status s;
  
  MPI_Init ( & argc , & argv ) ;

  MPI_Comm_size ( MPI_COMM_WORLD , & numProcs ) ;
  MPI_Comm_rank ( MPI_COMM_WORLD , & miId ) ;
  
   // En primer lugar se comprueba si el numero de procesos es valido
  if( numProcs < 2 ) {
    if ( miId == 0 ) {
      fprintf( stderr, "\n" );
      fprintf( stderr, "Error: Al menos se deben iniciar dos procesos\n" );
      fprintf( stderr, "\n" );
    }
    MPI_Finalize();
    return( -1 );
  }

  
  if ( miId == 0) {
    printf( "Dame un numero: ");
    scanf("%i", & n); // al ser un entero n, se pone %i, en caso de ser float %f
    
    for(i=1; i < numProcs; i++){
      MPI_Send(& n, 1, MPI_INT, i, 88, MPI_COMM_WORLD ); //Proceso 0 envía al resto de procesos
    }
    
  }else{
    MPI_Recv(& n, 1, MPI_INT, 0, 88, MPI_COMM_WORLD, &s ); //el resto de procesos reciben de proceso O
    printf("Soy el proceso %d, n = %d \n", miId, n);
  }
  
  MPI_Finalize () ;
  printf ( " Fin de programa \n " ) ;
  return 0;
}
 