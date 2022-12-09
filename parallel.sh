#!/bin/bash

#PBS -l select=4:ncpus=4:mem=2gb -l place=pack:excl
#PBS -l walltime=0:02:00
#PBS -q short_cpuQ

module load mpich-3.2
#module load openmpi-3.0.0

#export OMP_PROC_BIND=true 
#export OMP_PLACES=threads

#mpirun.actual -n 2  ./parallel_convex_hull/parallel $PBS_O_WORKDIR 1048576 0 0
#mpirun.actual -n 2 /home/leonardo.vicentini/parallel_convex_hull/parallel /home/leonardo.vicentini/parallel_convex_hull/ 1048576 0 8
mpirun.actual -n 4 ./parallel_convex_hull/parallel /home/$USER/parallel_convex_hull/ 4194304 0 4



#mpiexec -np 4 --map-by node:pe=4 --bind-to core ./parallel_convex_hull/parallel /home/$USER/parallel_convex_hull/ 1048576 0 4
#mpiexec -np 2 ./parallel_convex_hull/parallel /home/$USER/parallel_convex_hull/ 1048576 0 8
