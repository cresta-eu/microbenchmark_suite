// Copyright (c) 2013   HLRS, University of Stuttgart. All rights reserved.
//
// $COPYRIGHT$
//
// Additional copyrights may follow
//
// $HEADER$

#include "barrier.h"

inline int barrier (COMMTYPE comm)
{
	return MPI_Barrier (comm);
}
