#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <vector>
#include <algorithm>


using namespace std;


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int NUM_VALUES = 3;
    int sendCounts[NUM_VALUES] = { 2, 2, 1 }; // два целых и одно вещественное
    int displacements[NUM_VALUES] = { 0, 2 * sizeof(int), 2 * sizeof(int) + sizeof(double) };
    MPI_Datatype sendTypes[NUM_VALUES] = { MPI_INT, MPI_INT, MPI_DOUBLE };
    const int bufferSize = 100;
    char buffer[bufferSize];

    // Упаковка данных
    MPI_Pack(&rank, 1, MPI_INT, buffer, bufferSize, &displacements[0], MPI_COMM_WORLD);
    MPI_Pack(&size, 1, MPI_INT, buffer, bufferSize, &displacements[1], MPI_COMM_WORLD);
    double value = rank + 0.5;
    MPI_Pack(&value, 1, MPI_DOUBLE, buffer, bufferSize, &displacements[2], MPI_COMM_WORLD);

    // Определение размеров буферов и смещений для получения данных в главном процессе
    int* recvCounts = new int[size];
    int* recvDisplacements = new int[size];
    MPI_Gather(sendCounts, NUM_VALUES, MPI_INT, recvCounts, NUM_VALUES, MPI_INT, 0, MPI_COMM_WORLD);
    recvDisplacements[0] = 0;
    for (int i = 1; i < size; i++) {
        recvDisplacements[i] = recvDisplacements[i - 1] + recvCounts[i - 1];
    }
    int totalRecvCount = recvDisplacements[size - 1] + recvCounts[size - 1];
    char* recvBuffer = new char[totalRecvCount];

    // Сбор данных в главном процессе
    MPI_Gatherv(buffer, NUM_VALUES, MPI_PACKED, recvBuffer, recvCounts, recvDisplacements, MPI_PACKED, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Распаковка и вывод данных
        for (int i = 0; i < size; i++) {
            int r, s;
            double v;
            MPI_Unpack(&recvBuffer[recvDisplacements[i]], recvCounts[i], &s, &r, 1, MPI_INT, MPI_COMM_WORLD);
            MPI_Unpack(&recvBuffer[recvDisplacements[i]], recvCounts[i], &s, &s, 1, MPI_INT, MPI_COMM_WORLD);
            MPI_Unpack(&recvBuffer[recvDisplacements[i]], recvCounts[i], &s, &v, 1, MPI_DOUBLE, MPI_COMM_WORLD);
            std::cout << "Process " << r << " of " << s << " received value " << v << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
