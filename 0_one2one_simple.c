/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   0_one2one_simple.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/15 07:57:09 by pacovali      #+#    #+#                 */
/*   Updated: 2020/07/16 11:03:44 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
** simple one to one message passing
** compile: mpicc *0_one2one_simple.c -o test
** run mpirun -n 2 ./test
**
** New functions:
**	MPI_Init()		- start MPI
**	MPI_Comm_size()	- get amount of threads in current communicator
**	MPI_Comm_rank()	- get thread ID within current communicator
**	MPI_Get_processor_name()	- get name of node and name's length
**	MPI_Send()		- send message to the specified thread
**	MPI_Recv()		- receive message from the specified thread
**	MPI_Finalize()	- stop mpi communication
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

void	print_message(int *msg, int len) {
	printf("Message: {");
	for (int i = 0; i < len; i++) {
		printf(" %d,", msg[i]);
	}
	printf(" };\n");
}

int		main(void) {
	int			message[MSG_LEN];
	int			procs, rank, tag;
	tNodeName	name;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	if (procs != 2) { 
		exit(!!printf("Error: unsupported amount of processes (%d!=2)\n", procs));
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(name.text, &name.len);
	for (int i = 0; i < 10; i++) {
		if (rank == 0) {
			initialize_message(message, MSG_LEN);
			MPI_Send(message, MSG_LEN, MPI_INT, 1, 123, MPI_COMM_WORLD);
			printf("Sent message from %d to 1 with tag 123\n", rank);
			print_message(message, MSG_LEN);
		} else if (rank == 1) {
			MPI_Recv(message, MSG_LEN, MPI_INT, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Received message from 0 to d with tag 123\n", rank);
			print_message(message, MSG_LEN);
		}
	}
	MPI_Finalize();
}