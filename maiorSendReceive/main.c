#include <mpi.h>
#include <stdio.h>

#define ROTULO 42

int main(int argc, char **argv) {
  // Eiji Dogen RA: 22.219.027-4
  // Bruno Orlandin RA: 22.219.32-4

  // Initialize the MPI environment
  MPI_Init(&argc, &argv);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  
  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Definir numeros do vetor
  // Tamanho do vetor precisa ser multiplo de 3
  float numeros[9] = {1.0,2.3,12.4,1.2, 10.2, 4.2,13.1, 12.3, 100.1};
  int tamanho = 9;
  
  if (world_rank == 0) {

    int gritar = 1;

    // Procura maior numero local
    int i;
    float maior, temp;
    
    for (i = 0; i < tamanho/world_size; i++){
      if (i == 0){
        maior = numeros[i];
      }else{
        temp = numeros[i];
        if (temp > maior){
          maior = temp;
        }
      }
    }

    // Enviar para outros processos
    MPI_Send(&maior, 1, MPI_INT, 1, ROTULO, MPI_COMM_WORLD);
    MPI_Send(&maior, 1, MPI_INT, 2, ROTULO, MPI_COMM_WORLD);
    printf("Processo 0 enviou... \n");
    // Receber do processo 1
    float buffer = 0;
    MPI_Recv(&buffer, 1, MPI_INT, 1, ROTULO, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Processo 0 recebeu %f do processo 1\n", buffer);
    if (buffer > maior){gritar = 0;}

    // Receber do processo 2
    buffer = 0;
    MPI_Recv(&buffer, 1, MPI_INT, 2, ROTULO, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Processo 0 recebeu %f do processo 2\n", buffer);
    if (buffer > maior){gritar = 0;}

    // Print final
    if (gritar == 1){
      printf("Maior numero: %f (Processo 0)\n", maior);
    }
    
  } else if (world_rank == 1) {

    int gritar = 1;

    // Procura maior numero local
    int i;
    float maior, temp;
    
    for (i = tamanho/world_size; i < 2*(tamanho/world_size); i++){
      if (i == tamanho/world_size){
        maior = numeros[i];
      }else{
        temp = numeros[i];
        if (temp > maior){
          maior = temp;
        }
      }
    }

    // Receber do processo 0
    float buffer = 0;
    MPI_Recv(&buffer, 1, MPI_INT, 0, ROTULO, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (buffer > maior){gritar = 0;}
    printf("Processo 1 recebeu %f do processo 0\n", buffer);
    
    // Enviar para outros processos
    MPI_Send(&maior, 1, MPI_INT, 0, ROTULO, MPI_COMM_WORLD);
    MPI_Send(&maior, 1, MPI_INT, 2, ROTULO, MPI_COMM_WORLD);
    printf("Processo 1 enviou... \n");
    
    // Receber do processo 2
    buffer = 0;
    MPI_Recv(&buffer, 1, MPI_INT, 2, ROTULO, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (buffer > maior){gritar = 0;}
    printf("Processo 1 recebeu %f do processo 2\n", buffer);

    // Print final
    if (gritar == 1){
      printf("Maior numero: %f (Processo 1)\n", maior);
    }
    
    
  } else if (world_rank == 2) {

    int gritar = 1;

    // Procura maior numero local
    int i;
    float maior, temp;
    
    for (i = 2*(tamanho/world_size); i < 3*(tamanho/world_size); i++){
      if (i == 2*(tamanho/world_size)){
        maior = numeros[i];
      }else{
        temp = numeros[i];
        if (temp > maior){
          maior = temp;
        }
      }
    }

    // Receber do processo 0
    float buffer = 0;
    MPI_Recv(&buffer, 1, MPI_INT, 0, ROTULO, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (buffer > maior){gritar = 0;}
    printf("Processo 2 recebeu %f do processo 0\n", buffer);

    // Receber do processo 1
    buffer = 0;
    MPI_Recv(&buffer, 1, MPI_INT, 1, ROTULO, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (buffer > maior){gritar = 0;}
    printf("Processo 2 recebeu %f do processo 1\n", buffer);

    // Enviar para outros processos
    MPI_Send(&maior, 1, MPI_INT, 0, ROTULO, MPI_COMM_WORLD);
    MPI_Send(&maior, 1, MPI_INT, 1, ROTULO, MPI_COMM_WORLD);
    printf("Processo 2 enviou... \n");

    // Print final
    if (gritar == 1){
      printf("Maior numero: %f (Processo 2)\n", maior);
    }

  
  } else {
    printf("Processo nao definido.\n");
  }

  // Finalize the MPI environment.
  MPI_Finalize();
}