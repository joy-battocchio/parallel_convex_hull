#!/bin/bash

#PBS -l select=1:ncpus=1:mem=1gb
#PBS -l walltime=0:02:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 1 ./parallel_convex_hull/serial 4096
