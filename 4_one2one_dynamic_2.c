/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   4_one2one_dynamic_2.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/15 10:37:06 by pacovali      #+#    #+#                 */
/*   Updated: 2020/07/16 11:14:18 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <time.h>

/*
** receive a message without prior message length knowledge
** receive message meta data before message itself
** no need to allocate a buffer of size MAX_LEN beforehand
** compile: mpicc 4_one2one_dynamic_2.c -o test
** run    : mpirun -np 2 --hostfile my_host ./test
**
** New functions:
**	MPI_Probe()		- only receives message meta (MPI_Stat struct),
**		used before actual message passing to figure message length
*/

int		MAX_LEN =  10;

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
	int			procs, rank, tag=123;
	
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	if (procs != 2) { 
		exit(!!printf("Error: unsupported amount of processes (%d!=2)\n", procs));
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		int		destination = 1;
		int		send_len = random_len(MAX_LEN);
		int		*message = (int*)malloc(sizeof(int) * send_len);

		initialize_message(message, send_len);
		MPI_Send(message, send_len, MPI_INT, destination, tag, MPI_COMM_WORLD);
		print_message("Sent", message, send_len);
		free(message);
	} else {
		int			origin = 0;
		int			receive_len;
		MPI_Status	stat;

		MPI_Probe(0, tag, MPI_COMM_WORLD, &stat);
		MPI_Get_count(&stat, MPI_INT, &receive_len);
		int	*message = (int*)malloc(sizeof(int) * receive_len);
		MPI_Recv(message, receive_len, MPI_INT, origin, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		print_message("Received", message, receive_len);
		free(message);
	}
	return (0);
}