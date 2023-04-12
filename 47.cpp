/*MPIBegin47.В каждом процессе дан набор из K + 1 числа, где K — количество
процессов.Используя функцию MPI_Alltoallv, переслать в каждый процесс
два числа из каждого набора; при этом в процесс 0 надо переслать послед_ние два числа, в процесс 1 — два числа, предшествующих последнему, …,
в последний процесс — первые два числа каждого набора.В каждом про_цессе вывести полученные числа.
*/
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
	cout << "first rank:" << r << endl;
	for (int i = 0; i < n; i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}
void print(int* a, int m, int r) {
	cout << "second rank:" << r << endl;
	for (int i = 0; i < m; i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}
void printl(int* a) {
	for (int i = 0; i < sizeof(a); i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	srand(rank);

	const int n = size + 1;
	int* a=new int [n], *scount= new int[n], * rcount = new int[n], * sdispls= new int[n], *rdispls =new int[n], * b=new int [n*2];



	//int  b, tag = 0;
	MPI_Datatype datatype = MPI_INT;
	MPI_Comm comm = MPI_COMM_WORLD;
	int j = n - 1;

	for (int i = 0; i < n; i++) {
		a[i] = rank * i * 4 + i;
	}

	for (int i = 0; i < n; i++) {
		j--;
		scount[i] = 2;
		rcount[i] = 2;
		sdispls[i] = j;
		rdispls[i] = i*2;
		
	}

	printn(a, n, rank);

	MPI_Alltoallv(a, scount, sdispls, datatype, b, rcount, rdispls, datatype, comm);
	int m = n * 2 - 2;
	print(b, m, rank);
	delete[] a;
	delete[] scount;
	delete[] rcount;
	delete[] rdispls;
	delete[] sdispls;
	MPI_Finalize();
	return 0;

}
