#include <iostream>
#include <mpi.h>
#include <cmath>

int is_prime(int n)
{
    if (n <= 1)
        return 0;
    if (n <= 3)
        return 1;
    if (n % 2 == 0 || n % 3 == 0)
        return 0;

    int i = 5;
    while (i * i <= n)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
        i += 6;
    }
    return 1;
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, num_procs;
    double start_time, end_time;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int range_per_process = 1000000 / num_procs;
    int start = rank * range_per_process;
    int end = (rank + 1) * range_per_process;

    double total_time = 0.0;
    int runs = 1000;

    for (int i = 0; i < runs; i++)
    {
        int local_count = 0;

        MPI_Barrier(MPI_COMM_WORLD);
        double start_time = MPI_Wtime();

        for (int i = start; i < end; i++)
        {
            if (is_prime(i))
            {
                local_count++;
            }
        }

        int global_count[num_procs];
        MPI_Gather(&local_count, 1, MPI_INT, global_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

        double end_time = MPI_Wtime();
        total_time += (end_time - start_time);
    }

    if (rank == 0)
    {
        std::cout << "Average prime counting time over " << runs << " runs: "
                  << total_time / runs << " seconds." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
