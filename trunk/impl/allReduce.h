// Copyright (c) 2013   HLRS, University of Stuttgart. All rights reserved.
//
// $COPYRIGHT$
//
// Additional copyrights may follow
//
// $HEADER$

#ifdef MPI
#include "mpi.h"
#define COMMTYPE MPI_Comm
#endif

#ifdef UPC
#define COMMTYPE int
#endif

//int MPI_Allreduce ( void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
extern inline int allReduce (void *sendbuf, void *recvbuf, int count, int dataType, int op, COMMTYPE comm);
