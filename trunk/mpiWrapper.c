// Copyright (c) 2013   HLRS, University of Stuttgart. All rights reserved.
//
// $COPYRIGHT$
//
// Additional copyrights may follow
//
// $HEADER$

#include "mpiWrapper.h"

MPI_Datatype dataTypeArray1 [] = {MPI_BYTE, MPI_PACKED, MPI_CHAR, MPI_SHORT, MPI_INT, MPI_LONG, MPI_FLOAT, MPI_DOUBLE, MPI_LONG_DOUBLE, MPI_UNSIGNED_CHAR};
MPI_Op opType1 [] = {MPI_MAX, MPI_MIN, MPI_SUM, MPI_PROD, MPI_LAND, MPI_BAND, MPI_LOR, MPI_BOR, MPI_LXOR, MPI_BXOR, MPI_MAXLOC, MPI_MINLOC}; 

//This function writeup should be rethinked
inline int init (int argc, char *argv [], void* WTimeFlag, int* rank, int* size, MPI_Comm *comm)
{
	int flag, myFlag, mySize;
	//these lines should be wrie again
	MPI_Init (&argc, &argv);
	*comm = MPI_COMM_WORLD;
	MPI_Comm_size(*comm, size);
	MPI_Comm_rank(*comm, rank);
	MPI_Comm_get_attr (*comm, MPI_WTIME_IS_GLOBAL, WTimeFlag, &flag);

	if (!flag)
		*((long *)WTimeFlag) = 1;

	return 1;
}


inline int callSend (void *buf, int count, int Dtype, int dest, int tag, MPI_Comm comm)
{
	return MPI_Send (buf, count, dataTypeArray1 [Dtype], dest, tag, comm); 
}

//int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)

inline int callRecv (void *buf, int count, int Dtype, int source, int tag, MPI_Comm comm)
{
	MPI_Status status;

	return MPI_Recv (buf, count, dataTypeArray1 [Dtype], source, tag, comm, &status);
}

inline int getCommSize (MPI_Comm comm, int *size)
{
	return MPI_Comm_size (comm, size);
}

inline int getTypeSize (int datatype, int *size)
{
	return MPI_Type_size (dataTypeArray1 [datatype], size);
}

inline int getRank (MPI_Comm comm, int *rank)
{
	return MPI_Comm_rank (comm, rank);
}

inline int callCommDup (MPI_Comm comm, MPI_Comm *newComm)
{
	return MPI_Comm_dup (comm, newComm);	
}

inline int callFinalize ()
{
	return MPI_Finalize ();
}

inline int callCommFree (MPI_Comm *comm)
{
	return MPI_Comm_free (comm);
}

inline int callCommSplit (MPI_Comm comm, int color, int key, MPI_Comm *newComm)
{
	return MPI_Comm_split (comm, color, key, newComm);
}

inline int callGather(void *sendbuf, int sendcnt, int sendtype, void *recvbuf, int recvcnt, int recvtype, int root, MPI_Comm comm)
{
	return MPI_Gather (sendbuf, sendcnt, dataTypeArray1 [sendtype], recvbuf, recvcnt, dataTypeArray1 [recvtype], root, comm);
}
 
inline int callBroadcast (void *buffer, int count, int datatype, int root, MPI_Comm comm)
{
	MPI_Bcast (buffer, count, dataTypeArray1 [datatype], root, comm);
}

inline int callReduce (void *sendbuf, void *recvbuf, int count, int datatype, int op, int root, MPI_Comm comm)
{
	MPI_Reduce (sendbuf, recvbuf, count, dataTypeArray1 [datatype], opType1 [op], root, comm);
}

inline int callBarrier (MPI_Comm comm)
{
	MPI_Barrier (comm);
}
