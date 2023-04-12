#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <vector>
#include <algorithm>


struct MyStruct {
    int int1;
    int int2;
    float float1;
} ;

int main(int argc, char** argv) {
    int my_rank, num_procs;
    MPI_Datatype my_type, type[3] = { MPI_INT, MPI_INT, MPI_FLOAT };
    MPI_Aint disp[3];
    int blocklen[3];
    MyStruct my_data = { 1, 2, 3.14 };
    MyStruct recv_data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    
    disp[0] = offsetof(MyStruct, int1);
    disp[1] = offsetof(MyStruct, int2);
    disp[2] = offsetof(MyStruct, float1);
    blocklen[0] = 1;
    blocklen[1] = 1;
    blocklen[2] = 1;

 
    MPI_Type_create_struct(3, blocklen, disp, type,&my_type);
    MPI_Type_commit(&my_type);

    if (my_rank == 0) {
      
        for (int i = 1; i < num_procs; i++) {
            MPI_Recv(&recv_data, 1, my_type, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Received data from process %d: %d %d %f\n", i, recv_data.int1, recv_data.int2, recv_data.float1);
        }
    }
    else {
     
        MPI_Send(&my_data, 1, my_type, 0, 0, MPI_COMM_WORLD);
    }


    MPI_Type_free(&my_type);
    MPI_Finalize();
    return 0;
}
