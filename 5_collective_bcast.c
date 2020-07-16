/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   5_collective_bcast.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/15 11:43:55 by pacovali      #+#    #+#                 */
/*   Updated: 2020/07/16 11:20:18 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include <mpi.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <time.h>

/*
** broadcast one message to many processes
** compile : mpicc 5_collective.c -o test
** run     : mpirun -np 8 --hostfile my_host ./test
**
** New functions:
**	MPI_Bcast()		- distribute message from master thread to all threads in communicator
**	MPI_Barrier()	- wait for all threads from current communicator to reach this point
*/

int		MAX_LEN = 10;
int		MAX_PROCS = 8;

int		random_len(int max) {
	srand(time(NULL));
	return (rand() / (float)RAND_MAX * MAX_LEN);
}

void	initialize_message(int *msg, int len) {
	for (int i = 0; i < len; i++) {
		msg[i] = i * i * (i % 2 ? 1 : -1);
	}
}

void	print_message(char *s, int *msg, int len) {
	printf("%-8s: {", s);
	for (int i = 0; i < len; i++) {
		printf(" %d,", msg[i]);
	}
	printf(" };\n");
}

int		main(void) {
	int		procs, rank, tag=123, master=0;
	int		length = random_len(MAX_LEN);
	int		*message = (int*)malloc(sizeof(int) * length);
	char	*text;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	if (procs > MAX_PROCS) {
		exit(!!printf("Error: maximum threads qty exceeded(%d)\n", procs));
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == master) {
		initialize_message(message, length);
		text = "Sent";
	} else {
		text = "Received";
	}
	MPI_Bcast(message, length, MPI_INT, master, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);									// syncronize processes
	print_message(text, message, length);
	free(message);
	MPI_Finalize();
	return (0);
}