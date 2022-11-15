#!/bin/bash

#PBS -l select=4:ncpus=8:mem=2gb
#PBS -l walltime=0:10:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 4 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/output/output_ 128
