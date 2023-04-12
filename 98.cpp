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
    int rank, size;
    int K = 0; // Число процессов
    int A = 0; // Число A в текущем процессе

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Проверка на нечетное число процессов
    if (size % 2 == 0 || size<5 || size >9) {
        if (rank == 0) {
            cout << "An odd number of processes K = 2N + 1 (1 < N < 5) required!" << endl;
        }
        MPI_Finalize();
        return 1;
    }

    K = size;//7
    int n = (K - 1) / 2;//3
    A = rank * rank; // Пример числа A, можно заменить на свою логику
    cout << rank << "-rank a:" << A<<endl;
    // Создание графовой топологии
    int nnodes = K;
    int* index = new int[nnodes];
    int* edges = new int[(K * 2) ]; // Максимальное количество ребер для N-лучевой звезды
    int reorder = 0; // Не переупорядочивать ранги процессов

    // Заполнение массива index
    index[0] = n;
    for (int i = 1; i < nnodes; ++i) {
        if (i % 2 == 1 ) {
            index[i] = index[i-1]+2; 
        }
        else {
            index[i] = index[i - 1] + 1;
        }
    }
    int l = 0;
    for (int i = 0; i < K; i++) {
        
        if (i % 2 == 0) {
            if (i == 0) {
                for (int j = 0; j < n; j++)
                {
                    edges[j] = 2 * j + 1;
                    l++;
                }
            }
            else {
                edges[l] = i - 1;
                l++;
            }
        }
        else {
            edges[l] = 0; 
            l++;
            edges[l] = i + 1; 
            l++;
        }
    }

    MPI_Comm comm_graph;
    MPI_Graph_create(MPI_COMM_WORLD, size, index, edges, reorder, &comm_graph);

    
    int nneighbors;
    MPI_Graph_neighbors_count(comm_graph, rank, &nneighbors);
    int* neighbors = new int[nneighbors];
    MPI_Graph_neighbors(comm_graph, rank, nneighbors, neighbors);
    std::cout << "process:  " << rank << " has :  " << nneighbors << " neihghbors" << std::endl;

    
    // Пересылка числа A между соседними процессами
    for (int i = 0; i < nneighbors; ++i) {
        int dest = neighbors[i];
        int recvA;
   
        MPI_Sendrecv(&A, 1, MPI_INT, dest, 0, &recvA, 1, MPI_INT, dest, 0, comm_graph, MPI_STATUS_IGNORE);
        
        cout  << dest << " -->  " <<rank<< " = " <<  recvA  << std::endl;
     

    }
    
    
    MPI_Comm_free(&comm_graph);
    MPI_Finalize();
    delete[] index;
    delete[] edges;
    delete[] neighbors;

    return 0;
}
