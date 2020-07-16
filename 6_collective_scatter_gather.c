/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   6_collective_scatter_gather.c                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/15 13:02:53 by pacovali      #+#    #+#                 */
/*   Updated: 2020/07/16 11:57:11 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include <mpi.h>
# include <time.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

/*
** Distribute and combine data among processes
** combine equal parts of data from all processes - gather()
** compile : mpicc 6_collective_scatter_gather.c -o test
** run     : mpirun -np 8 --hostfile my_host ./test
**
** New functions:
**	MPI_Scatter()	- distribute equal chunks of data among processes
**	MPI_Gather()	- combine equal chunks of data into one array
*/

int		MAX_LEN = 5;
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

// double each element
void	do_stuff(int *recv_message, int receive_len) {
	for (int i = 0; i < receive_len; i++) {
		recv_message[i] *= 2;
	}
}

int		main(void) {
	int				procs, rank, master = 0;
	int				*send_message, *recv_message;
	int				send_len, receive_len;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	if (procs > MAX_PROCS) {
		exit(!!printf("Error: max processes quatity exceeded(%d>%d)\n", procs, MAX_PROCS));
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	send_len = random_len(MAX_LEN) * MAX_PROCS + MAX_PROCS;		// make sure there is at least one element per receiver process
	receive_len = send_len / MAX_PROCS;
	if (rank == 0) {
		send_message = (int*)malloc(sizeof(int) * send_len);
		initialize_message(send_message, send_len);
		print_message("Generate", send_message, send_len);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	recv_message = (int*)malloc(sizeof(int) * receive_len);
	MPI_Scatter(send_message, receive_len, MPI_INT, recv_message, receive_len, MPI_INT, master, MPI_COMM_WORLD);
	print_message("Received", recv_message, receive_len);

	do_stuff(recv_message, receive_len);

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(recv_message, receive_len, MPI_INT, send_message, receive_len, MPI_INT, master, MPI_COMM_WORLD);
	if (rank == 0) {
		print_message("Output", send_message, send_len);
		free(send_message);
	}
	free(recv_message);
	MPI_Finalize();
	return (0);
}