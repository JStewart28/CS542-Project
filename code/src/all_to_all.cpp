#include <stdio.h>
#include <iostream>

#include "mpi.h"

const int DATA_SIZE = 16000;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, num_procs;
    double start_time, end_time;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    char *send_data = new char[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; i++)
    {
        send_data[i] = 1; 
    }

    char *recv_data = new char[num_procs * DATA_SIZE];

    double total_time = 0.0;
    int runs = 1000;

    for (int i = 0; i < runs; i++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        double start_time = MPI_Wtime();

        MPI_Alltoall(send_data, DATA_SIZE, MPI_CHAR, recv_data, DATA_SIZE, MPI_CHAR, MPI_COMM_WORLD);

        double end_time = MPI_Wtime();
        total_time += (end_time - start_time);
    }

    if (rank == 0)
    {
        std::cout << "Average All-to-all time over " << runs << " runs: "
                  << total_time / runs << " seconds." << std::endl;
    }

    delete[] recv_data;
    delete[] send_data;

    MPI_Finalize();
    return 0;
}
