/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   7_collective_allgather.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/15 14:03:04 by pacovali      #+#    #+#                 */
/*   Updated: 2020/07/16 11:25:49 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include <mpi.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <time.h>

int		MAX_LEN = 5;
int		MAX_PROCS = 8;

/*
** Send available chunks from each processes to all processes (all-to-all)
** chunks are received in rank order
** compile : mpicc 7_collective_allgather.c -o test
** run     : mpirun -np 8 --hostfile my_host ./test
**
** New functions:
**	MPI_Allgather()	- send chunk to all threads and receive chunks from all sthreads
*/


void	print_message(char *s, int *msg, int len) {
	printf("%-8s: {", s);
	for (int i = 0; i < len; i++) {
		printf(" %d,", msg[i]);
	}
	printf(" };\n");
}

int		main(void) {
	int		procs, rank, tag=123;
	int		*message;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	if (procs > MAX_PROCS) {
		exit(!!printf("Error: max processes quatity exceeded(%d>%d)\n", procs, MAX_PROCS));
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("Rank: %d\n", rank);
	MPI_Barrier(MPI_COMM_WORLD);
	message = (int*)malloc(sizeof(int) * procs);
	MPI_Allgather(&rank, 1, MPI_INT, message, 1, MPI_INT, MPI_COMM_WORLD);
	print_message("Received", message, procs);
	MPI_Finalize();
	return (0);
}