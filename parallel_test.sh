#!/bin/bash

#PBS -l select=4:ncpus=4:mpiprocs=4:ompthreads=4:mem=4gb -l place=pack:excl
#PBS -l walltime=0:20:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 4 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 4194304 0 4
