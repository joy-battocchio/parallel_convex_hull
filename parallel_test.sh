#!/bin/bash

#PBS -l select=4:ncpus=8:mem=2gb -l place=pack:excl
#PBS -l walltime=0:02:00
#PBS -q short_cpuQ

module load mpich-3.2
#module load openmpi-3.0.0

#export OMP_PROC_BIND=true 
#export OMP_PLACES=threads

mpirun.actual -n 4 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 1048576 0 8
#mpiexec -np 2 --map-by node:pe=8 --bind-to core ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 1048576 0 8
