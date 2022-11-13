#!/bin/bash

#PBS -l select=8:ncpus=4:mem=4gb
#PBS -l walltime=0:10:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 32 ./parallel_convex_hull/parallel_test
