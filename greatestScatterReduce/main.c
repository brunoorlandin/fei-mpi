#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float randFloat(){
  return ((float)rand()) / ((float)RAND_MAX);
}

float randFloatRange(float inicialRange, float finalRange){
  return randFloat() * (finalRange-inicialRange) + inicialRange;
}

void fillArray(float *array, int arraySize){  
  for(int count=0; count<arraySize; count++){
    array[count] = randFloatRange(1,100);
  }
}

void printArray(float *array, int arraySize){  
  for(int i=0; i<arraySize; i++){
    printf("Vetor[%d] = %.2f\n", i+1,array[i]);
  }
}

float getGreatest(float *array, int arraySize){
  float greatest = array[0];
  for (int i = 1; i<arraySize; i++){
    if (greatest < array[i]){
      greatest = array[i];
    }
  }

  return greatest;
}

int main(int argc, char **argv) {
  // Seed the rand function to the time
  srand( time(NULL) );

  // Initialize the MPI environment
  MPI_Init(&argc, &argv);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  
  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  
  // Size needs to be proporcional to the number of ranks
  int size = 15;
  int numberPerProcess = size/world_size;
  
  if (world_rank == 0) {  
    float data[size];
    int arraySize = sizeof (data) / sizeof (data[0]);

    fillArray(data, arraySize);

    printf("Vetor criado no rank 0:\n");
    printArray(data,size);

    float resp[arraySize];

    MPI_Scatter(
      data,
      numberPerProcess,
      MPI_FLOAT,
      resp,					
      numberPerProcess,
      MPI_FLOAT,
      0,
      MPI_COMM_WORLD
    );

    printf("\nResposta rank %d\n", world_rank);
    printArray(resp,numberPerProcess);

    float localGreatest = getGreatest(resp, numberPerProcess);
    printf("Maior local: %.2f\n", localGreatest);

    float globalGreatest;
    
    MPI_Reduce(
      &localGreatest,
      &globalGreatest,
      1,
      MPI_FLOAT,
      MPI_MAX,
      0,
      MPI_COMM_WORLD
    );

    printf("\nMaior global: %.2f\n", globalGreatest);
    
  } else {
      float data[1];

      float resp[100];

      MPI_Scatter(
        data,
        numberPerProcess,
        MPI_FLOAT,
        resp,
        numberPerProcess,
        MPI_FLOAT,
        0,
        MPI_COMM_WORLD
      );

      printf("\nResposta rank %d\n", world_rank);
      printArray(resp,numberPerProcess);

      float localGreatest = getGreatest(resp, numberPerProcess);
      printf("Maior local: %.2f\n", localGreatest);

      float globalGreatest;
      
      MPI_Reduce(
        &localGreatest,
        &globalGreatest,
        1,
        MPI_FLOAT,
        MPI_MAX,
        0,
        MPI_COMM_WORLD
      );
    }

  // Finalize the MPI environment.
  MPI_Finalize();
}