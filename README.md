# parallel_convex_hull
Parallel implementation of divide et impera algorithm for convex hull

![](https://github.com/joy-battocchio/parallel_convex_hull/blob/main/Serial_plotting.gif)

![](https://github.com/joy-battocchio/parallel_convex_hull/blob/main/Parallel_plotting.gif)

[Benchmark spreadsheet](https://docs.google.com/spreadsheets/d/1AfCg2CoH7EQCHtPAphq1yIljPKH1MVXR2ypcxep76lI/edit?usp=sharing)


## How to run the code
General steps:

+ `cd parallel_convex_hull` (Moving inside the directory)
+ `module load mpich-3.2` (Loading the MPICH module)
+ `make clean` (Cleaning older binaries and output files)

For the serial program:

+ `make serial` (Compiling the serial program)
+ `make sub_serial` (Submitting the job)

For the parallel program:
+ `make parallel` (Compiling the parallel program)
+ `make sub_parallel` (Submitting the job)
+ `make sub_benchmark` (Submitting 10 jobs in a row)






