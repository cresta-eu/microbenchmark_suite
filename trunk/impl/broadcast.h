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

//int MPI_Bcast ( void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm )
extern inline int broadcast (void *buffer, int count, int dataType, int root, COMMTYPE comm);
