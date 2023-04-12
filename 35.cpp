
#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <vector>
#include <algorithm>


using namespace std;

void printn(int* a, int n, int r) {
	cout << " rank:" << r << endl;
	for (int i = 0; i < n; i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}

void print1(int a, int r) {
	cout << " rank:" << r <<"   mail: " <<a<< endl;
}
int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//srand(rank);

	const int n = size;
	int* a=new int [n];
	int b;
	int count = 1, tag = 0;
	MPI_Datatype datatype = MPI_INT;
	MPI_Comm comm = MPI_COMM_WORLD;

	for (int i = 0; i < n; i++) {
			a[i] = rand() % 10;
		}
	if(rank==0) printn(a, n, rank);	

	MPI_Scatter(a, count, datatype, &b, count, datatype, 0, comm);
	print1(b, rank);
	
	MPI_Finalize();
	return 0;

}
