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

extern inline int gather(void *sendbuf, int sendcnt, int sendtype, void *recvbuf, int recvcnt, int recvtype, int root, COMMTYPE comm);

