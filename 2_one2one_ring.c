/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   2_one2one_ring.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/15 09:14:07 by pacovali      #+#    #+#                 */
/*   Updated: 2020/07/16 10:51:33 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** compile: mpicc 2_one2one_ring.c -o test
** run: mpirun -n 8 --hostfile my_host ./test
** simple one to one message passing with ping-pong
** first printed element of message		- receiving rank
** second printed element of message	- sending rank
**
** New functions:
*/

# include <mpi.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

int		MSG_LEN = 5;
int		MAX_PROCS = 8; // limit form the host file

typedef struct sNodeName {
	int		len;
	char	text[MPI_MAX_PROCESSOR_NAME];
} tNodeName;

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
	int			message[MSG_LEN];
	int			procs, rank, tag=123, partner;
	tNodeName	name;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	if (procs > MAX_PROCS) {
		exit(!!printf("Error: unsupported amount of processes (%d)\n", procs));
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	partner = (rank - 1 + 8) % 8;
	MPI_Get_processor_name(name.text, &name.len);
	if (rank == 0) { initialize_message(message, MSG_LEN); }
	if (rank != 0) {
		MPI_Recv(message, MSG_LEN, MPI_INT, partner, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		print_message("Received", message, MSG_LEN);
	}
	message[0] = rank;
	message[1] = partner;
	MPI_Send(message, MSG_LEN, MPI_INT, (rank + 1) % 8, tag, MPI_COMM_WORLD);
	if (rank == 0) {
		MPI_Recv(message, MSG_LEN, MPI_INT, partner, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		print_message("Received", message, MSG_LEN);
	}
	MPI_Finalize();
}