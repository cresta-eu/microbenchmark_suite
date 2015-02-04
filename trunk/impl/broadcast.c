// Copyright (c) 2013   HLRS, University of Stuttgart. All rights reserved.
//
// $COPYRIGHT$
//
// Additional copyrights may follow
//
// $HEADER$

#include "broadcast.h"

MPI_Datatype dataTypeArray [] = {MPI_BYTE, MPI_PACKED, MPI_CHAR, MPI_SHORT, MPI_INT, MPI_LONG, MPI_FLOAT, MPI_DOUBLE, MPI_LONG_DOUBLE, MPI_UNSIGNED_CHAR};
MPI_Op opArray [] = {MPI_MAX, MPI_MIN, MPI_SUM, MPI_PROD, MPI_LAND, MPI_BAND, MPI_LOR, MPI_BOR, MPI_LXOR, MPI_BXOR, MPI_MAXLOC, MPI_MINLOC};

inline int broadcast (void *buffer, int count, int dataType, int root, MPI_Comm comm)
{
	return MPI_Bcast (buffer, count, dataTypeArray [dataType], root, comm); 
}

