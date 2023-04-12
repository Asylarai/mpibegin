#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <vector>
#include <algorithm>

#define K 4
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int count[2] = { 0,0 };
    double value = 0.0;
    int position = 0;
    char buffer[1024];

    if (rank == 0) {
        
        int data[K - 1][2] = { {1, 2 }, {4, 5 }, {7, 8} };
        double dat[K - 1] = { 3.14,6.28,9.45 };

        for (int i = 0; i < K - 1; i++) {
            MPI_Pack(&data[i][0], 2, MPI_INT, buffer, 1024, &position, MPI_COMM_WORLD);
            MPI_Pack(&dat[i], 1, MPI_DOUBLE, buffer, 1024, &position, MPI_COMM_WORLD);
        }

      
        MPI_Bcast(buffer, position, MPI_PACKED, 0, MPI_COMM_WORLD);
    }
    else {
       
        MPI_Bcast(buffer, 1024, MPI_PACKED, 0, MPI_COMM_WORLD);

        
        for (int i = 0; i < K - 1; i++) {
            MPI_Unpack(buffer, 1024, &position, &count, 2, MPI_INT, MPI_COMM_WORLD);
            MPI_Unpack(buffer, 1024, &position, &value, 1, MPI_DOUBLE, MPI_COMM_WORLD);

            std::cout << "Process " << rank << ": " << count[0] << " " << count[1] << " " << value << std::endl;
        }
    }

    MPI_Finalize();

    return 0;
}
