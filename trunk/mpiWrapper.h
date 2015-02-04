// Copyright (c) 2013   HLRS, University of Stuttgart. All rights reserved.
//
// $COPYRIGHT$
//
// Additional copyrights may follow
//
// $HEADER$

#ifndef _MPI_WRAPPER_H_
#define _MPI_WRAPPER_H_

#include "mpi.h"
#include "stdlib.h"

extern inline int init (int argc, char *argv [], void* WTimeFlag, int* rank, int* size, MPI_Comm *comm);

extern inline int callSend (void *buf, int count, int Dtype, int dest, int tag, MPI_Comm comm);

extern inline int callRecv (void *buf, int count, int Dtype, int source, int tag, MPI_Comm comm);

extern inline int getCommSize (MPI_Comm comm, int *size);

extern inline int getRank (MPI_Comm comm, int *rank);

//int MPI_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm)
extern inline int callCommDup (MPI_Comm comm, MPI_Comm *newComm);

extern inline int getTypeSize (int datatype, int *size);

//int MPI_Comm_free(MPI_Comm *comm)
extern inline int callCommFree (MPI_Comm *comm);

//int MPI_Finalize( void )
extern inline int callFinalize ();

//int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm)
extern inline int callCommSplit (MPI_Comm comm, int color, int key, MPI_Comm *newcomm);

extern inline int callGather (void *sendbuf, int sendcnt, int sendtype, void *recvbuf, int recvcnt, int recvtype, int root, MPI_Comm comm);

//int MPI_Bcast( void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
extern inline int callBroadcast (void *buffer, int count, int datatype, int root, MPI_Comm comm);

//int MPI_Reduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
extern inline int callReduce (void *sendbuf, void *recvbuf, int count, int datatype, int op, int root, MPI_Comm comm);

//int MPI_Barrier( MPI_Comm comm )
extern inline int callBarrier (MPI_Comm comm); 


#endif

