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

//int MPI_Allgather(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
extern inline int allGather (void* sendbuf, int sendcnt, int sendtype, void* recvbuf, int recvcnt, int recvtype, COMMTYPE comm);
