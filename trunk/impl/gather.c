// Copyright (c) 2013   HLRS, University of Stuttgart. All rights reserved.
//
// $COPYRIGHT$
//
// Additional copyrights may follow
//
// $HEADER$

#include "gather.h"

MPI_Datatype dataTypeArray2 [] = {MPI_BYTE, MPI_PACKED, MPI_CHAR, MPI_SHORT, MPI_INT, MPI_LONG, MPI_FLOAT, MPI_DOUBLE, MPI_LONG_DOUBLE, MPI_UNSIGNED_CHAR};

inline int gather(void *sendbuf, int sendcnt, int sendtype, void *recvbuf, int recvcnt, int recvtype, int root, COMMTYPE comm)
{
	return MPI_Gather (sendbuf, sendcnt, dataTypeArray2 [sendtype], recvbuf, recvcnt, dataTypeArray2 [recvtype], root, comm); 
}
