# Intro-to-OpenMPI

Exercises on basic OpenMPI commands.

Exercises are a part of [MPI tutorial](https://mpitutorial.com/)

![OpenMPI_logo](https://upload.wikimedia.org/wikipedia/commons/6/6f/Open_MPI_logo.png)

## Covered functions:

### 0_one2one_simple.c - simple one to one message passing

* `MPI_Init()`					- start MPI
* `MPI_Comm_size()`				- get amount of threads in current communicator
* `MPI_Comm_rank()`				- get thread ID within current communicator
* `MPI_Get_processor_name()`	- get name of node and name's length
* `MPI_Send()`					- send message to the specified thread
* `MPI_Recv()`					- receive message from the specified thread
* `MPI_Finalize()`				- stop mpi communication

### 1_one2one_ping_pong.c - simple one to one message passing with ping-pong

### 2_one2one_ring.c - simple one to one message passing around all threads

### 3_one2one_dynamic.c - receive a message without prior message length knowledge using a preallocated buffer

* `MPI_Get_count()`		- get message length from MPI_Stat structure

### 4_one2one_dynamic_2.c - receive a message without prior message length knowledge without preallocated buffer

* `MPI_Probe()`			- only receives message meta (MPI_Stat struct), used before actual message passing to figure message length

### 5_collective_bcast.c - broadcast one message to many processes

* `MPI_Bcast()`			- distribute message from master thread to all threads in communicator
* `MPI_Barrier()`		- wait for all threads from current communicator to reach this point

### 6_collective_scatter_gather.c - distribute and compine data among processes

* `MPI_Scatter()`		- distribute equal chunks of data among processes
* `MPI_Gather()`		- combine equal chunks of data into one array

### 7_collective_allgather.c - send available chunks from each processes to all processes(all-to-all)

* `MPI_Allgather()`		- send chunk to all threads and receive chunks from all sthreads

### 8_collective_reduce.c - many to many communication

* `MPI_Reduse()`		- send available chuncs from each process to master process (all-to-one)
* `MPI_Allreduce()`		- send available chuncs from each process to all processes (all-to-all)

### 9_group_management.c - Simple group management, custom communicators

* `MPI_Comm_duplicate()`		- duplicate a communicator to not manipulate original one
* `MPI_Comm_split()`			- create a new communicator based on rank's color and key
* `MPI_Comm_create()`			- create a new communicator based on group, involves all threads in communicator
* `MPI_Comm_create_group()`		- create a new communicator based on group, involves only threads in group
* `MPI_Group_union()`			- create a union group from two parameter groups
* `MPI_Group_intersection()`	- create an intersection group
* `MPI_Group_incl()`			- create a new group from an array of ranks

# Each file contains compilation and execution instructions
Each file is self contained to make it simpler to track what is going on, for easier compilation and execution. This means some of the helper functions repeat through files.
