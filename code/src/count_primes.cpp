#include <iostream>
#include <cmath>
#include <unistd.h>

#include "mpi.h"

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

    int is_oversubscribed = 0;
    if (argc != 2) {
        printf("Please pass oversubscribe flag of 1 or 0\n");
        MPI_Finalize();
        return 0;
    }
    if (atoi(argv[1]) == 1) {
        is_oversubscribed = 1;
    }

    int start = 0;
    int end = 10000;

    // Number of times to run 'runs' runs to calculate the std dev
    int num_runs = 16;
    int sample_size = 100;
    int runs = 1000;
    double* avg_times = new double[sample_size];

    for (int r = 0; r < num_runs; r++)
    {
        for (int w = 0; w < sample_size; w++) 
        {
            double total_time = 0.0;
            int local_count = 0;

            for (int j = 0; j < runs; j++)
            {
                MPI_Barrier(MPI_COMM_WORLD);
                start_time = MPI_Wtime();

                for (int i = start; i < end; i++)
                {
                    if (is_prime(i))
                    {
                        local_count++;
                    }
                }

                int global_count[num_procs];
                MPI_Gather(&local_count, 1, MPI_INT, global_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

                end_time = MPI_Wtime();
                total_time += (end_time - start_time);
            }

            // Record avg times
            avg_times[w] = total_time / runs;
        }

        // Print results
        if (rank == 0)
        {
            // Get average time of the sample_runs runs
            double average = 0.0;
            for (int i = 0; i < sample_size; i++)
            {
                average += avg_times[i];
            }
            average /= sample_size;

            double std_dev = 0.0;
            for(int i = 0; i < sample_size; i++ )
            {
                std_dev += (avg_times[i] - average) * (avg_times[i] - average);
            }
            std_dev /= sample_size;
            std_dev = sqrt(std_dev);

            // Get hostname
            char hostname[1024];
            hostname[1023] = '\0';
            gethostname(hostname, 1023);

            // Print everything
            // name, hostname, num_procs, oversubscribed, avg_time, std_dev
            printf("count_primes, %s, %d, %d, %e, %e\n",
                    hostname, num_procs, is_oversubscribed, average, std_dev);
        }
    }

    delete[] avg_times;

    MPI_Finalize();
    return 0;
}

/*
for (int i = 0; i < runs; i++)
    {
        int local_count = 0;

        MPI_Barrier(MPI_COMM_WORLD);
        start_time = MPI_Wtime();

        for (int i = start; i < end; i++)
        {
            if (is_prime(i))
            {
                local_count++;
            }
        }

        int global_count[num_procs];
        MPI_Gather(&local_count, 1, MPI_INT, global_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

        end_time = MPI_Wtime();
        total_time += (end_time - start_time);
    }

    if (rank == 0)
    {
        std::cout << "Average prime counting time over " << runs << " runs: "
                  << total_time / runs << " seconds." << std::endl;
    }

    MPI_Finalize();
    return 0;

*/