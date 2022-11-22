Strange behavior:

```c
//in cloud_generator_testing(vector *v)
int x_test = 663;
int y_test = 8764;   
point test = (point){.x = x_test, .y = y_test}; 
v->pfVectorSet(v, 70, &test);

int x_test2 = 456;
int y_test2 = 777;
point test2 = (point){.x = x_test2, .y = y_test2};
v->pfVectorSet(v, 65, &test2);
```

```shell
point 64: ( 99 ; 198 )
point 65: ( 0 ; 99 )
point 66: ( 99 ; 198 )
point 67: ( 99 ; 198 )
point 68: ( 99 ; 198 )
point 69: ( 99 ; 198 )
point 70: ( 0 ; 8764 )
```

---------



points with negative x:             always correct
first 1/2 points with positive x:   always correct
points here in the middle:          wrong
last 1/2 points with positive x:    always correct

# Runs

#PBS -l select=16:ncpus=1:mem=4gb
mpirun.actual -n 16 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 128
no threads 
HULL OK


#PBS -l select=32:ncpus=1:mem=4gb
mpirun.actual -n 32 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 128
no threads 
only 30 mpi processes spawned
31th not spawned (so output_31.txt empty) VERY COMMON PATTERN
so hull not present in output file


#PBS -l select=32:ncpus=1:mem=6gb
mpirun.actual -n 32 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 128
no threads 
only 30 mpi processes spawned
31th not spawned
output_30.txt and _31.txt not even present
so hull not present in output file
in error file:
Fatal error in MPI_Recv: Unknown error class, error stack:
MPI_Recv(200).........................: MPI_Recv(buf=0x7ffd092da410, count=33, dtype=USER<struct>, src=7, tag=0, MPI_COMM_WORLD, status=0x1) failed
MPIDU_Complete_posted_with_error(1137): Process failed


#PBS -l select=32:ncpus=16:mem=6gb
error file: walltime excedeed 
correct number of processes spawned


#PBS -l select=16:ncpus=1:mem=4gb
mpirun.actual -n 16 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 128
no thread
HULL OK


#PBS -l select=1:ncpus=32:mem=4gb
mpirun.actual -n 32 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 128
no thread
HULL OK


#PBS -l select=2:ncpus=16:mem=4gb
mpirun.actual -n 32 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 128
no thread
HULL OK


#PBS -l select=2:ncpus=32:mem=4gb
mpirun.actual -n 64 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 128
Fatal error in PMPI_Scatter: Unknown error class, error stack:
PMPI_Scatter(804).....................: MPI_Scatter(sbuf=0x7fffb74967c0, scount=2, dtype=USER<struct>, rbuf=0x7fffb7495f80, rcount=2, dtype=USER<struct>, root=0, MPI_COMM_WORLD) failed
MPIR_Scatter_impl(624)................: 
MPIR_Scatter(585).....................: 
MPIR_Scatter_intra(190)...............: 
MPIDU_Complete_posted_with_error(1137): Process failed
Only 40 processes spawned

#PBS -l select=4:ncpus=16:mem=4gb
mpirun.actual -n 64 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 128
various behaviours in different runs


#PBS -l select=4:ncpus=32:mem=4gb
mpirun.actual -n 128 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 128
ERROR:
[proxy:0:0@hpc-c11-node18.unitn.it] HYD_pmcd_pmip_control_cmd_cb (pm/pmiserv/pmip_cb.c:885): assert (!closed) failed
[proxy:0:0@hpc-c11-node18.unitn.it] HYDT_dmxu_poll_wait_for_event (tools/demux/demux_poll.c:76): callback returned error status
[proxy:0:0@hpc-c11-node18.unitn.it] main (pm/pmiserv/pmip.c:206): demux engine error waiting for event
[proxy:0:1@hpc-c11-node23.unitn.it] HYD_pmcd_pmip_control_cmd_cb (pm/pmiserv/pmip_cb.c:885): assert (!closed) failed
[proxy:0:1@hpc-c11-node23.unitn.it] HYDT_dmxu_poll_wait_for_event (tools/demux/demux_poll.c:76): callback returned error status
[proxy:0:1@hpc-c11-node23.unitn.it] main (pm/pmiserv/pmip.c:206): demux engine error waiting for event
[proxy:0:2@hpc-c10-node01.unitn.it] HYD_pmcd_pmip_control_cmd_cb (pm/pmiserv/pmip_cb.c:885): assert (!closed) failed
[proxy:0:2@hpc-c10-node01.unitn.it] HYDT_dmxu_poll_wait_for_event (tools/demux/demux_poll.c:76): callback returned error status
[proxy:0:2@hpc-c10-node01.unitn.it] main (pm/pmiserv/pmip.c:206): demux engine error waiting for event
[mpiexec@hpc-c11-node18.unitn.it] HYDT_bscu_wait_for_completion (tools/bootstrap/utils/bscu_wait.c:76): one of the processes terminated badly; aborting
[mpiexec@hpc-c11-node18.unitn.it] HYDT_bsci_wait_for_completion (tools/bootstrap/src/bsci_wait.c:23): launcher returned error waiting for completion
[mpiexec@hpc-c11-node18.unitn.it] HYD_pmci_wait_for_completion (pm/pmiserv/pmiserv_pmci.c:218): launcher returned error waiting for completion
[mpiexec@hpc-c11-node18.unitn.it] main (ui/mpich/mpiexec.c:344): process manager error waiting for completion

and in output:
===================================================================================
=   BAD TERMINATION OF ONE OF YOUR APPLICATION PROCESSES
=   PID 84241 RUNNING AT hpc-c11-node17.unitn.it
=   EXIT CODE: 8
=   CLEANING UP REMAINING PROCESSES
=   YOU CAN IGNORE THE BELOW CLEANUP MESSAGES
===================================================================================


Because MPIPROCESS == cloud_size 
we must add the handling of those edge cases



#PBS -l select=4:ncpus=32:mem=4gb
mpirun.actual -n 128 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 1024

WORKED ON 1ST RUN
ERROR ON 2ND: 
Fatal error in PMPI_Scatter: Unknown error class, error stack:
PMPI_Scatter(804).....................: MPI_Scatter(sbuf=0x7ffd5856bfe0, scount=8, dtype=USER<struct>, rbuf=0x7ffd58567f40, rcount=8, dtype=USER<struct>, root=0, MPI_COMM_WORLD) failed
MPIR_Scatter_impl(624)................: 
MPIR_Scatter(585).....................: 
MPIR_Scatter_intra(190)...............: 
MPIDU_Complete_posted_with_error(1137): Process failed


empty outputs:
31
47
49
50
51
54
55
58
62
63
127


#PBS -l select=4:ncpus=32:mem=2gb
mpirun.actual -n 128 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 1024

first run OK
second run OK
third run OK
fourth run OK
fifth run OK
sixth run OK
seventh run OK
eitgh run OK
ninth run OK



#PBS -l select=8:ncpus=32:mem=2gb
mpirun.actual -n 256 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 1024

first run OK



#PBS -l select=8:ncpus=64:mem=2gb
mpirun.actual -n 512 ./parallel_convex_hull/parallel_test /home/$USER/parallel_convex_hull/ 1024

first run OK

second run ERROR:
Fatal error in MPI_Ssend: Unknown error class, error stack:
MPI_Ssend(152)..................: MPI_Ssend(buf=0x7fff0e487d30, count=3, dtype=USER<struct>, dest=229, tag=0, MPI_COMM_WORLD) failed
MPIDI_EagerSyncNoncontigSend(86): failure occurred while attempting to send an eager message
MPIDI_CH3_iSendv(34)............: Communication error with rank 229
Fatal error in MPI_Ssend: Unknown error class, error stack:
MPI_Ssend(152)..................: MPI_Ssend(buf=0x7ffea081a730, count=3, dtype=USER<struct>, dest=277, tag=0, MPI_COMM_WORLD) failed
MPIDI_EagerSyncNoncontigSend(86): failure occurred while attempting to send an eager message
MPIDI_CH3_iSendv(34)............: Communication error with rank 277
Fatal error in MPI_Ssend: Unknown error class, error stack:
MPI_Ssend(152)..................: MPI_Ssend(buf=0x7ffd88066030, count=3, dtype=USER<struct>, dest=301, tag=0, MPI_COMM_WORLD) failed
MPIDI_EagerSyncNoncontigSend(86): failure occurred while attempting to send an eager message
MPIDI_CH3_iSendv(34)............: Communication error with rank 301
Fatal error in MPI_Ssend: Unknown error class, error stack:
MPI_Ssend(152)..................: MPI_Ssend(buf=0x7fff9071bca0, count=3, dtype=USER<struct>, dest=341, tag=0, MPI_COMM_WORLD) failed
MPIDI_EagerSyncNoncontigSend(86): failure occurred while attempting to send an eager message
MPIDI_CH3_iSendv(34)............: Communication error with rank 341
Fatal error in MPI_Ssend: Unknown error class, error stack:
MPI_Ssend(152)..................: MPI_Ssend(buf=0x7ffe3df857b0, count=3, dtype=USER<struct>, dest=461, tag=0, MPI_COMM_WORLD) failed
MPIDI_EagerSyncNoncontigSend(86): failure occurred while attempting to send an eager message
MPIDI_CH3_iSendv(34)............: Communication error with rank 461

third run:
error file empty 
but no HULL
some output.txt are empty 
511 empty for example (last file empty is a common pattern)

1:1
1:2
1:4
