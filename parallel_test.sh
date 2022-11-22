#!/bin/bash

#PBS -l select=1:ncpus=8:mpiprocs=8:mem=2gb -l place=pack:excl
#PBS -l walltime=0:20:00
#PBS -q short_cpuQ

module load mpich-3.2
mpirun.actual -n 8 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 16777216
#mpirun.actual -n 8 xterm -e gdb ./parallel_convex_hull/parallel_test -ex run /home/$USER/parallel_convex_hull/ 131072
