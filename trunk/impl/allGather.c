// Copyright (c) 2013   HLRS, University of Stuttgart. All rights reserved.
//
// $COPYRIGHT$
//
// Additional copyrights may follow
//
// $HEADER$

#include "allGather.h"

MPI_Datatype dataTypeAllGather [] = {MPI_BYTE, MPI_PACKED, MPI_CHAR, MPI_SHORT, MPI_INT, MPI_LONG, MPI_FLOAT, MPI_DOUBLE, MPI_LONG_DOUBLE, MPI_UNSIGNED_CHAR};

inline int allGather(void *sendbuf, int sendcnt, int sendtype, void *recvbuf, int recvcnt, int recvtype, COMMTYPE comm)
{
	return MPI_Allgather(sendbuf, sendcnt, dataTypeAllGather [sendtype], recvbuf, recvcnt, dataTypeAllGather [recvtype], comm);
}

