#! /usr/bin/bash
echo "Please close all other programs before running"

echo "all_to_all 1"
mpirun -n 1 all_to_all 0 >> run_Aureolin.out
echo "all_to_all 2"
mpirun -n 2 all_to_all 0 >> run_Aureolin.out
echo "all_to_all 4"
mpirun -n 4 all_to_all 0 >> run_Aureolin.out
echo "all_to_all 6"
mpirun -n 6 all_to_all 0 >> run_Aureolin.out
echo "all_to_all 8"
mpirun -n 8 all_to_all 0 >> run_Aureolin.out
echo "all_to_all 16"
mpirun -n 16 --oversubscribe all_to_all 1 >> run_Aureolin.out
echo "all_to_all 24"
mpirun -n 24 --oversubscribe all_to_all 1 >> run_Aureolin.out
echo "all_to_all 64"
mpirun -n 32 --oversubscribe all_to_all 1 >> run_Aureolin.out
# More processes than this will cause an out of memory error

echo "count_primes 1"
mpirun -n 1 count_primes 0 >> run_Aureolin.out
echo "count_primes 2"
mpirun -n 2 count_primes 0 >> run_Aureolin.out
echo "count_primes 4"
mpirun -n 4 count_primes 0 >> run_Aureolin.out
echo "count_primes 6"
mpirun -n 6 count_primes 0 >> run_Aureolin.out
echo "count_primes 8"
mpirun -n 8 count_primes 0 >> run_Aureolin.out
echo "count_primes 16"
mpirun -n 16 --oversubscribe count_primes 1 >> run_Aureolin.out
echo "count_primes 24"
mpirun -n 24 --oversubscribe count_primes 1 >> run_Aureolin.out
echo "count_primes 32"
mpirun -n 32 --oversubscribe count_primes 1 >> run_Aureolin.out
echo "count_primes 64"
mpirun -n 64 --oversubscribe count_primes 1 >> run_Aureolin.out
echo "count_primes 128"
mpirun -n 128 --oversubscribe count_primes 1 >> run_Aureolin.out

echo "Done!"
