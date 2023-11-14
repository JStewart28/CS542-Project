#!/bin/bash
#SBATCH --job-name mpi
#SBATCH --mail-user jastewart@unm.edu
#SBATCH --mail-type FAIL,TIME_LIMIT
#SBATCH --output mpi.out
#SBATCH --error mpi.err
#SBATCH --nodes 1
#SBATCH --partition debug
#SBATCH --time 00:01:00

spack load openmpi+legacylaunchers
mpirun -n 1 all_to_all 0
mpirun -n 2 all_to_all 0
mpirun -n 4 all_to_all 0
mpirun -n 6 all_to_all 0
mpirun -n 8 all_to_all 0
mpirun -n 16 all_to_all 0
mpirun -n 24 --oversubscribe all_to_all 1
mpirun -n 32 --oversubscribe all_to_all 1
mpirun -n 64 --oversubscribe all_to_all 1
mpirun -n 128 --oversubscribe all_to_all 1

mpirun -n 1 count_primes 0
mpirun -n 2 count_primes 0
mpirun -n 4 count_primes 0
mpirun -n 6 count_primes 0
mpirun -n 8 count_primes 0
mpirun -n 16 count_primes 0
mpirun -n 24 --oversubscribe count_primes 1
mpirun -n 32 --oversubscribe count_primes 1
mpirun -n 64 --oversubscribe count_primes 1
mpirun -n 128 --oversubscribe count_primes 1
