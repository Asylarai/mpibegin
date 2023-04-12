#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <vector>

using namespace std;

void print(int* a,int n,int r) {
    cout << "bastanky rank:" << r << endl;
    for (int i = 0; i < n; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;
}
void oprint(int* a, int n, int r) {
    cout << "ozgergen rank:" << r << endl;
    for (int i = 0; i < n; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;
}

int main(int argc, char* argv[]) {
    MPI_Init(NULL, NULL);

    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   srand(rank);
   int n = size - 1, c;
   vector<int> b;
    int* a = new int[n],*d=new int[n];
    for (int i = 0; i < n; i++)
    {
        a[i] = rand()%5;
    }
    
    int count = 1, tag = 0;
    MPI_Datatype datatype = MPI_INT;
    MPI_Comm comm = MPI_COMM_WORLD;
    int buffer_size = (MPI_BSEND_OVERHEAD + sizeof(int));
    print(a, n, rank);

    int l = 0;
        for (int j = 0; j < n; j++)
        {
            if (rank == j) {
                l++;
            }
            MPI_Send(&a[j], count, datatype, l, tag, comm);
           // printf("from  rank %d out of %d processors - %d \n ", rank,l, a[j]);
            MPI_Recv(&c, count, datatype, l, tag, comm, MPI_STATUS_IGNORE);
            b.push_back(c);
            d[j] = b[j];
            printf("otpravil: %d, poluchil: %d,i:  %d  - mail: %d\n",l, rank,j, b[j]);
            l++;
        }
       
        oprint(d, n, rank);
	
	MPI_Finalize();
	return 0;
}
