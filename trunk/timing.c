// Copyright (c) 2013   HLRS, University of Stuttgart. All rights reserved.
//
// $COPYRIGHT$
//
// Additional copyrights may follow
//
// $HEADER$

#include "timing.h"

double getTime ()
{
	double currentTime;

	currentTime = MPI_Wtime ();
        return currentTime;
}

