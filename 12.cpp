#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <locale.h>


using namespace std;


int main(int argc, char* argv[]) {
    MPI_Init(NULL, NULL);

    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = size, b;
    int* a = new int[n];
    for (int i = 0; i < n; i++)
    {
        a[i] = rand() % 5;
    }

    int count = 1, dest = size - 1, tag = 0;
    MPI_Datatype datatype = MPI_INT;
    MPI_Comm comm = MPI_COMM_WORLD;
    int buffer_size = (MPI_BSEND_OVERHEAD + sizeof(double)*count);
   
    if (rank == 0) {
        int j = 0;
        cout << " array:\n"; 
        int* message_buffer = (int*)malloc(buffer_size);
    MPI_Buffer_attach(&message_buffer, buffer_size);
        for (int i = dest; i > 0; i--) {

            MPI_Bsend(&a[j], count, datatype, i, tag, MPI_COMM_WORLD);
            cout << "i:" << j << " - " << a[j] << endl;;
            j++;

        }  MPI_Buffer_detach(&message_buffer, &buffer_size);

    }
    else {
        MPI_Recv(&b, count, datatype, 0, tag, comm, MPI_STATUS_IGNORE);

        printf("from  rank %d out of 0 processors -  %d \n ",
            rank, b);
    }




    MPI_Finalize();
    return 0;
}
