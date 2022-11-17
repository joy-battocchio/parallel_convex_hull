#!/bin/bash

#PBS -l select=32:ncpus=32:mem=6gb
#PBS -l walltime=0:20:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 32 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 128
