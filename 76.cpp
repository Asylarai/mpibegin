#include <iostream>
#include <mpi.h>

using namespace std;
int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
  
    
    const int K = size-1; 
    double *data=new double[K];
    double recv_data;
    srand(rank);
    int N = rank == rand()%2;
    cout << rank << "-rank  N: " << N<<endl;
    MPI_Comm comm;
    MPI_Comm_split(MPI_COMM_WORLD, N, rank, &comm); 
    for (int i = 0; i < K; i++)
    {
        data[i] = rand() % 100 * 0.1;
        
    }
    if (rank == 0) {
        cout << "data:  ";
        for (int i = 0; i < K; i++)
        {
            cout << data[i] << " ";
        }
        cout << endl;
        
    }
    MPI_Scatter(data, 1, MPI_DOUBLE, &recv_data, 1, MPI_DOUBLE, 0, comm);   
    if (N == 1) {
        cout << "Process " << rank << " received " << recv_data << endl; // выводим полученные данные
    }
    MPI_Finalize();
    return 0;
}
/*#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <vector>
#include <algorithm>


using namespace std;


int main(int argc, char** argv) {
    int rank, procs;
 

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm comm = MPI_COMM_WORLD,newcomm;
    int N,n;
    srand(rank);
    MPI_Datatype type = MPI_DOUBLE;
    MPI_Comm_split(comm, N = rand()%2, rank, &newcomm);
    const int k = 3;
    double a[k],b;
    if (rank == 0) {
        for (int i = 0; i < k; i++) {
            a[i] = i+(rand()%8+1) / 10;        }
    }  MPI_Scatter(a, 1, type, &b, 1, type, 0, newcomm);
    if (rank != 0) {
          n = rand () % 2;
          if (n == 1) {
              MPI_Recv(&b, 1, type, 0, 0,newcomm, MPI_STATUS_IGNORE);
              cout << rank << "-rank  :" << b << endl;
          }
    }
    MPI_Finalize();
    return 0;
}*/