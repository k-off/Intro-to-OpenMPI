/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   3_one2one_dynamic.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/15 09:47:47 by pacovali      #+#    #+#                 */
/*   Updated: 2020/07/16 11:08:27 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <time.h>

/*
** receive a message without prior message length knowledge
** compile: mpicc 3_one2one_dynamic.c -o test
** run    : mpirun -np 2 --hostfile my_host ./test
**
** New functions:
**	MPI_Get_count()		- get message length from MPI_Stat structure
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
	int			procs, rank, tag = 123;
	MPI_Status	stat;
	int			message[MAX_LEN];
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	if (procs != 2) { 
		exit(!!printf("Error: unsupported amount of processes (%d!=2)\n", procs));
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		int len = random_len(MAX_LEN);
		initialize_message(message, len);
		MPI_Send(message, len, MPI_INT, 1, tag, MPI_COMM_WORLD);
		print_message("Sent", message, len);
	} else {
		int		received_len;
		MPI_Recv(message, MAX_LEN, MPI_INT, 0, tag, MPI_COMM_WORLD, &stat);
		MPI_Get_count(&stat, MPI_INT, &received_len);
		print_message("Received", message, received_len);
	}
	return (0);
}
