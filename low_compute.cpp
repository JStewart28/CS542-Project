#include <mpi.h>
#include <stdio.h>
#include <iostream>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, num_procs;
    double start_time, end_time;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int send_data = rank; // each process sends its own rank
    int recv_data[num_procs];
    double total_time = 0.0;
    int runs = 1000;

    for (int i = 0; i < runs; i++)
    {
        MPI_Barrier(MPI_COMM_WORLD); 
        double start_time = MPI_Wtime();

        MPI_Alltoall(&send_data, 1, MPI_INT, recv_data, 1, MPI_INT, MPI_COMM_WORLD);

        double end_time = MPI_Wtime();
        total_time += (end_time - start_time);
    }

    if (rank == 0)
    {
        std::cout << "Average All-to-all time over " << runs << " runs: "
                  << total_time / runs << " seconds." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
