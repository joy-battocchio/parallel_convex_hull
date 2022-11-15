#!/bin/bash

#PBS -l select=4:ncpus=2:mem=1gb
#PBS -l walltime=0:10:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 8 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/output/output_ 4096
