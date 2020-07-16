/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   8_collective_reduce.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/15 14:03:04 by pacovali      #+#    #+#                 */
/*   Updated: 2020/07/16 10:53:46 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include <mpi.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

int		MAX_LEN = 5;
int		MAX_PROCS = 8;

/*
** Many to many communication
** compile : mpicc 8_collective_reduce.c -o test
** run     : mpirun -np 8 --hostfile my_host ./test
** chunks are received in rank order
**
** New functions:
**	MPI_Reduse()	- send available chuncs from each process to master process (all-to-one)
**	MPI_Allreduce()	- send available chuncs from each process to all processes (all-to-all)
**
** Predefined in MPI reduce functions:
**	MPI_MAX		- Returns the maximum element.
**	MPI_MIN		- Returns the minimum element.
**	MPI_SUM		- Sums the elements.
**	MPI_PROD	- Multiplies all elements.
**	MPI_LAND	- Performs a logical and across the elements.
**	MPI_LOR		- Performs a logical or across the elements.
**	MPI_BAND	- Performs a bitwise and across the bits of the elements.
**	MPI_BOR		- Performs a bitwise or across the bits of the elements.
**	MPI_MAXLOC	- Returns the maximum value and the rank of the process that owns it.
**	MPI_MINLOC	- Returns the minimum value and the rank of the process that owns it.
*/

int		main(int ac, char **av) {
	int		procs, rank, tag=123, master = 0;
	int		size = 2;
	int		res = 0;
	int		is_allred = 0;

	if (ac != 2) {
		exit(!!printf("Usage: ./test parameter (-reduce - Reduce(), -allreduce - Allreduce())\n"));
	}
	is_allred |= -!strcmp("-allreduce", av[1]);
	is_allred |= !strcmp("-reduce", av[1]);
	if (!is_allred) {
		exit(!!(printf("Error: unsupported argument: \"%s\"\n", av[1])));
	}
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	if (procs > MAX_PROCS) {
		exit(!!printf("Error: max processes quatity exceeded(%d>%d)\n", procs, MAX_PROCS));
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("Rank: %d\n", rank);
	MPI_Barrier(MPI_COMM_WORLD);
	if (is_allred > 0)
		MPI_Reduce(&rank, &res, 1, MPI_INT, MPI_SUM, master, MPI_COMM_WORLD);
	else
		MPI_Allreduce(&rank, &res, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);	
	printf("Rank %d : rank_sum=%d\n", rank, res);
	MPI_Barrier(MPI_COMM_WORLD);
	if (!rank) {
		printf("Average rank %4.2f\n", (float)res / procs);
	}
	MPI_Finalize();
	return (0);
}
