#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <math.h>

#include "mpi.h"

const int DATA_SIZE = 4196;

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


    char *send_data = new char[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; i++)
    {
        send_data[i] = 1; 
    }

    char *recv_data = new char[num_procs * DATA_SIZE];

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
            for (int i = 0; i < runs; i++)
            {
                
                // Do benchmark
                MPI_Barrier(MPI_COMM_WORLD);
                double start_time = MPI_Wtime();

                MPI_Alltoall(send_data, DATA_SIZE, MPI_CHAR, recv_data, DATA_SIZE, MPI_CHAR, MPI_COMM_WORLD);

                double end_time = MPI_Wtime();
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
            printf("all_to_all, %s, %d, %d, %e, %e\n",
                    hostname, num_procs, is_oversubscribed, average, std_dev);
        }
    }

    delete[] recv_data;
    delete[] send_data;
    delete[] avg_times;

    MPI_Finalize();
    return 0;
}
