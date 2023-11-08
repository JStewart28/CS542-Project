#!/bin/bash

# cd into the runscripts directory
cd $(dirname "$0")

# build binaries
cd ..
mkdir build
cd build
cmake ..
make
cd ../runscripts/

NAME=$(hostname)
NCPU=$(nproc)
OUTF="../../results/run_$NAME.out" # Out file
BDIR="../build" # Build directory

echo "Please close all other programs before running"
echo "Running on $NAME with $NCPU CPUs"
echo "Writing results to $OUTF"
echo "--------------------------------------------------------------------------------"

for NPROCS in 1 2 #4 8 16 32 64 128
do
	if [ $NPROCS -le $NCPU ]
	then
		echo "all_to_all $NPROCS"
		mpirun -n $NPROCS $BDIR/all_to_all 0 >> $OUTF
		echo "count_primes $NPROCS"
		mpirun -n $NPROCS $BDIR/count_primes 0 >> $OUTF
	else
		echo "all_to_all $NPROCS"
		mpirun -n $NPROCS --oversubscribe $BDIR/all_to_all 1 >> $OUTF
		echo "count_primes $NPROCS"
		mpirun -n $NPROCS --oversubscribe $BDIR/count_primes 1 >> $OUTF
	fi
done

echo "--------------------------------------------------------------------------------"
echo "Done!"
