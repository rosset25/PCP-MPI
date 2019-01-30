#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define VECTOR_CORTO

// ============================================================================
// Declaracion de prototipos locales.

int esPrimo( long long num );
void construyeVector( int * numElementos, long long ** vectorNumeros );

// ============================================================================
int main( int argc, char *argv[] ) {
  int         miId, numProcs, i, numPrimosSec, numPrimosPar;
  double      t1, t2, ttSec, ttPar;
  long long   * vectorNumeros, dato; 
  int         dimVectorNumeros;
  char        miNombre[ MPI_MAX_PROCESSOR_NAME ];
  int         lonMiNombre;
  MPI_Status  status;
 
  
  
  // Comienza el codigo.
  MPI_Init( & argc, & argv );
  MPI_Comm_size( MPI_COMM_WORLD, & numProcs );
  MPI_Comm_rank( MPI_COMM_WORLD, & miId );

  // Imprime el nombre de los procesadores.
  MPI_Get_processor_name( miNombre, & lonMiNombre );
  printf( "Proceso %d  Se ejecuta en: %s\n", miId, miNombre );
  MPI_Barrier( MPI_COMM_WORLD );

  // El proceso 0 construye el vector de numeros que se va a estudiar.
  if( miId == 0 ) {
    construyeVector( & dimVectorNumeros, & vectorNumeros );
    printf( "El vector consta de %d numeros.\n", dimVectorNumeros );
    printf( "\n" );
  } 
  fflush( stdout );

  //
  // Implementacion secuencial (realizada en P0).
  // ============================================
  // 
  if( miId == 0 ) {
    t1 = MPI_Wtime();
    numPrimosSec = 0;
    for( i = 0; i < dimVectorNumeros; i++ ) {
      if( esPrimo( vectorNumeros[ i ] ) ) {
        numPrimosSec++;
      }
    }
    t2 = MPI_Wtime(); ttSec = t2 - t1;
    printf( "Implementacion secuencial.  Tiempo (s): %lf\n", ttSec );
    printf( "Numero de primos en el vector: %d\n", numPrimosSec );
    printf( "\n" );
  }
  fflush( stdout );

  // Comprueba que hay al menos 2 procesos para la version paralela.
  // La version paralela necesita al menos 2 procesos: 
  // 1 coordinador y al menos 1 trabajador.
  if( numProcs < 2 ) {
    fprintf( stderr, "ERROR: Debe haber al menos 2 procesos.\n" );
    MPI_Finalize();
    exit( -1 );
  }

  //
  // Implementacion Paralela.
  // ========================
  // 
  printf("Implementacion paralela\n");
  printf("empiezo\n");
  MPI_Barrier( MPI_COMM_WORLD );
  t1 = MPI_Wtime();
  numPrimosPar = 0;

  
  if (miId == 0) { //proceso distribuidor
    
    for(i=0; i < dimVectorNumeros; i++){
      dato = vectorNumeros[i]; //NOTA: ¿por qué no se puede poner directamente el vector? 
      MPI_Send(&dato, 1, MPI_LONG_LONG_INT, 1, 88, MPI_COMM_WORLD);
      //printf("proceso 0 envía a 1 los números...\n");
    }
    
    //proc 0 envía tarea envenenada
    MPI_Send(&dato, 1, MPI_LONG_LONG_INT, 1, 99, MPI_COMM_WORLD);
    
    //recibe el total de primos
    MPI_Recv(&numPrimosPar, 1, MPI_INT, 1, 88, MPI_COMM_WORLD, &status);
    //printf("cantidad de primos procesados: %d \n", numPrimosPar);
    
    
  }
  
  if (miId == 1){ //proceso que recibe
   
    MPI_Recv(&dato, 1, MPI_LONG_LONG_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD , &status) ; //ANY_TAG porque podría ser un vector 

    //mientras no se le envíe una tarea envenenada espera números
    while(status.MPI_TAG == 88){ 
      if(esPrimo(dato)){
	numPrimosPar++;
      }
      
      MPI_Recv(&dato, 1, MPI_LONG_LONG_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD , &status);
    }
    
    //acaba porque ya ha recibido la tarea envenenada y envía el resultado al proc 0
    MPI_Send(&numPrimosPar, 1, MPI_INT, 0, 88, MPI_COMM_WORLD); 
  }
  // ...
  // ...
  
  MPI_Barrier( MPI_COMM_WORLD );
  t2 = MPI_Wtime(); ttPar = t2 - t1;
  if( miId == 0 ) {
    printf( "Implementacion paralela.    Tiempo (s): %lf\n", ttPar );
    printf( "Numero de primos en el vector: %d\n", numPrimosPar );
  }

  // El proceso 0 destruye el vector de primos.
  if( miId == 0 ) {
    free( vectorNumeros );
  }
 
  // Fin de programa.
  if( miId == 0 ) {
    printf( "\n" ); fflush( stdout );
  }
  MPI_Barrier( MPI_COMM_WORLD );
  printf( "Proc: %d  Fin de programa\n", miId );
  MPI_Finalize();
  return 0;
}

// ============================================================================
int esPrimo( long long num ) {
  int primo;
  if( num < 2 ) {
    primo = 0;
  } else {
    primo = 1;
    long long i = 2;
    while( ( i < num )&&( primo ) ) { 
      primo = ( num % i != 0 );
      i++;
    }
  }
  return( primo );
}

// ============================================================================
void construyeVector( int * numElementos, long long ** vectorNumeros ) {
  int        numElems;
  long long  * vecNums;

#ifdef VECTOR_CORTO
  // Vector corto para pruebas cortas.
  numElems = 20;
  vecNums  = ( long long * ) malloc( numElems * sizeof( long long ) );

  vecNums[  0 ] = 3LL;  vecNums[  1 ] = 15LL; vecNums[  2 ] =  2LL;  
  vecNums[  3 ] = 4LL;  vecNums[  4 ] =  4LL; vecNums[  5 ] = 27LL; 
  vecNums[  6 ] = 7LL;  vecNums[  7 ] =  7LL; vecNums[  8 ] = 11LL; 
  vecNums[  9 ] = 4LL;  vecNums[ 10 ] =  4LL; vecNums[ 11 ] = 21LL;
  vecNums[ 12 ] = 4LL;  vecNums[ 13 ] =  8LL; vecNums[ 14 ] =  3LL;  
  vecNums[ 15 ] = 4LL;  vecNums[ 16 ] = 19LL; vecNums[ 17 ] =  4LL; 
  vecNums[ 18 ] = 5LL;  vecNums[ 19 ] =  7LL;  

#else
  // Vector largo para pruebas largas.
  numElems = 100;
  vecNums  = ( long long * ) malloc( numElems * sizeof( long long ) );

  vecNums[  0 ] = 2097593LL;   vecNums[  1 ] = 4LL;
  vecNums[  2 ] = 16785407LL;  vecNums[  3 ] = 27644437LL;
  vecNums[  4 ] = 16785407LL;  vecNums[  5 ] = 27LL; 
  vecNums[  6 ] = 7LL;         vecNums[  7 ] = 27644437LL;
  vecNums[  8 ] = 16785407LL;  vecNums[  9 ] = 4LL; 
  vecNums[ 10 ] = 4LL;         vecNums[ 11 ] = 2097593LL;
  vecNums[ 12 ] = 4LL;         vecNums[ 13 ] = 4LL; 
  vecNums[ 14 ] = 27644437LL;  vecNums[ 15 ] = 2097593LL;
  vecNums[ 16 ] = 4LL;         vecNums[ 17 ] = 7LL;
  vecNums[ 18 ] = 2097593LL;   vecNums[ 19 ] = 16785407LL;
  vecNums[ 20 ] = 4LL;         vecNums[ 21 ] = 2097593LL;
  vecNums[ 22 ] = 27644437LL;  vecNums[ 23 ] = 4LL; 
  vecNums[ 24 ] = 27644437LL;  vecNums[ 25 ] = 2097593LL;
  vecNums[ 26 ] = 4LL;         vecNums[ 27 ] = 27644437LL;
  vecNums[ 28 ] = 2097593LL;   vecNums[ 29 ] = 16785407LL;
  vecNums[ 30 ] = 5LL;         vecNums[ 31 ] = 16785407L;
  vecNums[ 32 ] = 4LL;         vecNums[ 33 ] = 2097593LL;
  vecNums[ 34 ] = 27644437LL;  vecNums[ 35 ] = 27LL; 
  vecNums[ 36 ] = 16785407LL;  vecNums[ 37 ] = 7LL;
  vecNums[ 38 ] = 27644437LL;  vecNums[ 39 ] = 16785407LL;
  vecNums[ 40 ] = 4LL;         vecNums[ 41 ] = 27644437LL;
  vecNums[ 42 ] = 16785407LL;  vecNums[ 43 ] = 4LL; 
  vecNums[ 44 ] = 27644437LL;  vecNums[ 45 ] = 16785407LL;
  vecNums[ 46 ] = 5LL;         vecNums[ 47 ] = 16785407LL;
  vecNums[ 48 ] = 16785407LL;  vecNums[ 49 ] = 16785407LL;
  vecNums[ 50 ] = 4LL;         vecNums[ 51 ] = 2097593LL;
  vecNums[ 52 ] = 4LL;         vecNums[ 53 ] = 2097593LL;
  vecNums[ 54 ] = 27644437LL;  vecNums[ 55 ] = 27LL; 
  vecNums[ 56 ] = 16785407LL;  vecNums[ 57 ] = 7LL;
  vecNums[ 58 ] = 27644437LL;  vecNums[ 59 ] = 16785407LL;
  vecNums[ 60 ] = 4LL;         vecNums[ 61 ] = 27644437LL;
  vecNums[ 62 ] = 16785407LL;  vecNums[ 63 ] = 4LL; 
  vecNums[ 64 ] = 27644437LL;  vecNums[ 65 ] = 16785407LL;
  vecNums[ 66 ] = 5LL;         vecNums[ 67 ] = 4097LL;
  vecNums[ 68 ] = 1025LL;      vecNums[ 69 ] = 4LL;
  vecNums[ 70 ] = 4LL;         vecNums[ 71 ] = 2097593LL;
  vecNums[ 72 ] = 16785407LL;  vecNums[ 73 ] = 27644437LL;
  vecNums[ 74 ] = 16785407LL;  vecNums[ 75 ] = 27LL; 
  vecNums[ 76 ] = 7LL;         vecNums[ 77 ] = 7LL;
  vecNums[ 78 ] = 27644437LL;  vecNums[ 79 ] = 4LL; 
  vecNums[ 80 ] = 4LL;         vecNums[ 81 ] = 2097593LL;
  vecNums[ 82 ] = 4LL;         vecNums[ 83 ] = 4LL; 
  vecNums[ 84 ] = 27644437LL;  vecNums[ 85 ] = 2097593LL;
  vecNums[ 86 ] = 4LL;         vecNums[ 87 ] = 7LL;
  vecNums[ 88 ] = 2097593LL;   vecNums[ 89 ] = 16785407LL;
  vecNums[ 90 ] = 5LL;         vecNums[ 91 ] = 2097593LL;
  vecNums[ 92 ] = 4LL;         vecNums[ 93 ] = 4LL;
  vecNums[ 94 ] = 2049LL;      vecNums[ 95 ] = 27LL;
  vecNums[ 96 ] = 4097LL;      vecNums[ 97 ] = 7LL;
  vecNums[ 98 ] = 4LL;         vecNums[ 99 ] = 2097593LL;
#endif

  // Devuelve el vector y su dimension.
  * numElementos = numElems;  * vectorNumeros = vecNums;
}
