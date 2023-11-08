#!/bin/bash

# run_desktop
#
# This script is meant to run our experiments on non-supercomputers.
# If you run this script without passing any command-line arguments,
# e.g. ./run_desktop.sh
# then it will oversubscribe up to 128 tasks. If you want to put an upper-
# bound on how many tasks it will oversubscribe, pass the upper-bound as the
# first (and only) command-line argument.
# e.g. to run at most 64 tasks: ./run_desktop.sh 64
UBOUND=128
if [ $# -eq 1 ]
then
	UBOUND=$1
fi
echo "Running with at most $UBOUND tasks"

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
OUTF="../../../results/run_$NAME.out" # Out file
BDIR="../build" # Build directory

echo "Please close all other programs before running"
echo "Running on $NAME with $NCPU CPUs"
echo "Writing results to $OUTF"
echo "--------------------------------------------------------------------------------"

for NPROCS in 1 2 4 6 8 16 32 64 128
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
