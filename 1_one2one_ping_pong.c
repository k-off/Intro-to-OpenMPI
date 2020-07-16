/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   1_one2one_ping_pong.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/15 07:57:09 by pacovali      #+#    #+#                 */
/*   Updated: 2020/07/16 10:51:23 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** simple one to one message passing with ping-pong
** compile: mpicc *0_one2one_ping_pong.c -o test
** run mpirun -n 2 ./test
** first printed element of message		- iteration
** second printed element of message	- rank
**
** New functions:
*/

# include <mpi.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

int		MSG_LEN = 5;

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
	int			procs, rank, tag, partner;
	tNodeName	name;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	if (procs != 2) { 
		exit(!!printf("Error: unsupported amount of processes (%d!=2)\n", procs));
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	partner = !rank;
	MPI_Get_processor_name(name.text, &name.len);
	initialize_message(message, MSG_LEN);
	for (int i = 0; i < 10; i++) {
		message[0] = i;
		message[1] = rank;
		if (rank == (i % 2)) {
			MPI_Send(message, MSG_LEN, MPI_INT, partner, 123, MPI_COMM_WORLD);
			print_message("Sent", message, MSG_LEN);
		} else if (rank != (i % 2)) {
			MPI_Recv(message, MSG_LEN, MPI_INT, partner, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			print_message("Received", message, MSG_LEN);
		}
	}
	MPI_Finalize();
}