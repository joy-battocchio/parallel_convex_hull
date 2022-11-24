#!/bin/bash

#PBS -l select=1:ncpus=4:mpiprocs=1:mem=2gb -l place=pack:excl
#PBS -l walltime=0:20:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 1 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 1024 1 4
