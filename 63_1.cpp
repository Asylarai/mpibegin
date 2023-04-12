#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <vector>
#include <algorithm>


using namespace std;




int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	srand(rank);
	
	struct buf_s {
		int b[2];
		//char a;
		double a;
	} ;

	struct buf_s buffer,buff, buf[7];
	MPI_Datatype type[2],buf_type;
	int* count=new int[2], disp[2];
	MPI_Comm comm = MPI_COMM_WORLD;

	type[0] = MPI_INT;
	count[0] = 2;
	disp[0] = offsetof(buf_s,b);
	type[1] = MPI_DOUBLE;
	count[1] = 1;
	disp[1] = offsetof(buf_s, a);

	MPI_Type_create_struct(2, count, disp, type, &buf_type);
	MPI_Type_commit(&buf_type);
	if (rank!=0) {
		
			buffer.b[0] = rank*rank;
			buffer.b[1] = rank *10+1;
			buffer.a = 0.2+rank;
			//buffer.a = 'ds';

		MPI_Send(&buffer, 1, buf_type, 0, 0, comm);
		
	}
	if (rank == 0) {
		int k = 0;
		for (int j = 1; j < size; j++) {
			
			MPI_Recv(&buff, 1, buf_type, j, 0, comm, MPI_STATUSES_IGNORE);
			buf[k] = buff;
			k++;
			cout << j << "-rank:\n" <<  buff.b[0]<<"  "<< buff.b[1] << "  "<< buff.a << "  " << endl;
			
		}
		cout << "0 rank poluchil:" << endl;
		for (int j = 0; j < size-1; j++) {
		cout << buf[j].b[0] << "  " << buf[j].b[1] << "  " << buf[j].a << "  "  ;

	}
	}
	
		
	
	MPI_Finalize();
	return 0;

}
