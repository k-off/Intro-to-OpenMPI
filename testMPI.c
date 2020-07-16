# include <mpi.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

/*
** use processor name to generate processor id
** ranks with the same processor id belong to the same node
** processor id is used in `color` field of MPI_Comm_split() function
*/
int		simple_hash(char *processor_name, int name_length) {
	int processor_id = 0;

	while (name_length) {
		name_length--;
		processor_id ^= (processor_name[name_length] << (name_length % 5 * 6));
	}
	return (processor_id);
}

int main (void) {
    int     procs, proc_rank, name_len, node_id = 0, data;
    char    proc_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
	data = -proc_rank;
	if (proc_rank) {
		MPI_Send(
			&data,					// data array
			1,						// array length
			MPI_INT,				// data type
			0,						// where to send
			proc_rank + 1,			// message tag
			MPI_COMM_WORLD);		// communicator
	} else {
		int		n = 1;				// sender id
		MPI_Recv(
			&data,					// data array
			1,						// array length
			MPI_INT,				// data type
			n,						// data source
			n + 1,					// data tag
			MPI_COMM_WORLD,			// communicator
			MPI_STATUS_IGNORE);		// message status
		printf("Received from: %d, tag: %d, message: %d\n", n, n + 1 , data);
	}
	MPI_Finalize();
    return (0);
}

/* local node and rank */
// MPI_Comm shmcomm;
// MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0,
//                     MPI_INFO_NULL, &shmcomm);
// int shmrank;
// MPI_Comm_rank(shmcomm, &shmrank);

/*  */
// int MPI_Comm_split_type(MPI_Comm comm, int split_type, int key, MPI_Info info, MPI_Comm *newcomm)

// MPI_Init(int ac, cahr **av);						- initialize MPI
// MPI_Comm_size(MPI_COMM_WORLD, int *procs);		- get amount of processes
// MPI_Comm_rank(MPI_COMM_WORLD, int *rank);		- get current rank
// MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, int *sharedmemorycomm); - split world communicator in communicators with shared memory
// MPI_Finalize(void);								- finalize MPI
