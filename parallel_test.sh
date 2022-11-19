#!/bin/bash

#PBS -l select=8:ncpus=32:mem=2gb
#PBS -l walltime=0:20:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 256 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 1024
