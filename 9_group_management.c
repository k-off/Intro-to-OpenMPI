/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   9_group_management.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/16 08:20:17 by pacovali      #+#    #+#                 */
/*   Updated: 2020/07/16 10:54:14 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include <mpi.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

/*
** Simple group management, custom communicators
** compile : mpicc 9_group_management.c -o test
** run     : mpirun -np 8 --hostfile my_host ./test
**
** New functions:
** MPI_Comm_duplicate()		- duplicate a communicator to not manipulate original one
** MPI_Comm_split()			- create a new communicator based on rank's color and key
** MPI_Comm_create()		- create a new communicator based on group, involves all threads in communicator
** MPI_Comm_create_group()	- create a new communicator based on group, involves only threads in group
** MPI_Group_union()		- create a union group from two parameter groups
** MPI_Group_intersection()	- create an intersection group
** MPI_Group_incl()			- create a new group from an array of ranks
*/

int MAX_PROCS = 8

int		main(void) {
	int	glob_size=-1, glob_rank=-1, odd_size=-1, odd_rank=-1, even_size=-1, even_rank=-1;
	MPI_Comm	glob_comm, even_comm, odd_comm;
	MPI_Group	glob_group, even_group, odd_group;

	MPI_Init(NULL, NULL);
	MPI_Comm_dup(MPI_COMM_WORLD, &glob_comm);
	MPI_Comm_size(glob_comm, &glob_size);
	if (glob_size > MAX_PROCS) {
		exit(!!printf("Error: max processes quatity exceeded(%d>%d)\n", glob_size, MAX_PROCS));
	}

	MPI_Comm_rank(glob_comm, &glob_rank);
	MPI_Comm_group(glob_comm, &glob_group);
	if (glob_rank >= 0)
		printf("Global rank (GR): %d\n", glob_rank);
	MPI_Barrier(glob_comm);

	/* create odd group and communicator */
	MPI_Comm_split(glob_comm, (glob_rank % 2 == 1), 111, &odd_comm);
	MPI_Comm_group(odd_comm, &odd_group);
	if (glob_rank % 2 == 1) {
		MPI_Comm_size(odd_comm, &odd_size);
		MPI_Comm_rank(odd_comm, &odd_rank);
	}
	if (odd_rank >= 0)
		printf("GR: %d, odd rank(OR): %d\n", glob_rank, odd_rank);
	MPI_Barrier(glob_comm);

	/* create even group and communicator */
	MPI_Group_incl(glob_group, glob_size / 2 + glob_size % 2, (int[]){0, 2, 4, 6}, &even_group);
	MPI_Comm_create_group(glob_comm, even_group, 123, &even_comm);
	if (glob_rank % 2 == 0) {
		MPI_Comm_size(even_comm, &even_size);
		MPI_Comm_rank(even_comm, &even_rank);
	}
	if (even_rank >= 0)
		printf("GR: %d, even rank(ER): %d\n", glob_rank, even_rank);
	MPI_Barrier(glob_comm);

	MPI_Finalize();
}