#!/bin/bash

#PBS -l select=4:ncpus=16:mpiprocs=8:ompthreads=8:mem=2gb -l place=pack:excl
#PBS -l walltime=0:20:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 8 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 1048576 0 8
