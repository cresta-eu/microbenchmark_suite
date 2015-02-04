// Copyright (c) 2013   HLRS, University of Stuttgart. All rights reserved.
//
// $COPYRIGHT$
//
// Additional copyrights may follow
//
// $HEADER$

#define str(s) #s
#define xstr(s) str(s)

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "timing.h"
#include "string.h"
#include "math.h"
#include xstr (collLib) 

#ifdef MPI
#include "mpiWrapper.h"
#ifndef COMMTYPE
#define COMMTYPE MPI_Comm
#endif
#endif

#ifdef UPC
#ifndef COMMTYPE
#define COMMTYPE int
#endif
#endif

#define NOOFREADINGS 1000
#define ROOT 0

typedef enum type {BYTE, PACKED, CHAR, SHORT, INT, LONG, FLOAT, DOUBLE, LONG_DOUBLE, UNSIGNED_CHAR} dataType;
typedef enum opType {MAX, MIN, SUM, PROD, LAND, BAND, LOR, BOR, LXOR, BXOR, MAXLOC, MINLOC} opArray; 

//This function gives information about skew between the ranks.
//skew points to the skew array.
void getClkSkew (double *skew, COMMTYPE comm)
{
	int rank, i, j;
	int commSize;
	double myTime, temp;
	double remoteTime;
	double latency;

	getCommSize (comm, &commSize);
	getRank (comm, &rank);
	
	skew [ROOT] = 0; //Skew of root rank to its self will be zero.
	
	if (rank == ROOT)
	{
		for (i = 1; i < commSize; i++)
		{
			skew [i] = 0;

			for (j = 0; j < NOOFREADINGS; j++)
			{
				myTime = getTime ();
				callSend (&myTime, 1, DOUBLE, i, 1024, comm);
				callRecv (&remoteTime, 1, DOUBLE, i, 1024, comm);
				latency = (getTime () - myTime) / 2;
				temp = (remoteTime - myTime) - latency;
				skew [i] += temp;
			}
		
			skew [i] /= NOOFREADINGS; 	
		}
	}
	else
	{
		for (j = 0; j < NOOFREADINGS; j++)
		{
			callRecv (&remoteTime, 1, DOUBLE, 0, 1024, comm);
			myTime = getTime ();
			callSend (&myTime, 1, DOUBLE, 0, 1024, comm);
		}
	}
}

//This function is resposible for testing the collective.
void timeCollective (double* skew,int count, COMMTYPE comm, dataType datatype, int delayRank, int delay)
{
	int i, dataTypeSize, size, rank;
	double minStart, maxEnd, diff, start, end;
	double minMaxAverage, localAverage, globalAverage, max, min, stdDev = 0;
	double allDiffs [NOOFREADINGS], dataInBytes;

	getTypeSize (datatype, &dataTypeSize);
	getCommSize(comm, &size);
	getRank (comm, &rank);
		
#ifdef ALLREDUCE
	void *sendBuf, *recvBuf;
	sendBuf = malloc (sizeof(float) * count);
	recvBuf = malloc (sizeof(float) * count);

	//warmup phase
	for (i = 0; i < 512; i++)
		allReduce (sendBuf, recvBuf, count, datatype, SUM, comm);
#endif

#ifdef BROADCAST
	void *buffer;
	buffer = malloc (dataTypeSize * count);	
	barrier (comm);
	
	//warmup phase
	for (i = 0; i < 512; i++)
		broadcast (buffer, count, datatype, ROOT, comm);
#endif

#ifdef GATHER
	void *sendBuf, *recvBuf;
	sendBuf = malloc (dataTypeSize * count);
	recvBuf = malloc (dataTypeSize * count * size);
	
	//warmup phase
	for (i = 0; i < 512; i++)
		gather (sendBuf, count, datatype, recvBuf, count, datatype, ROOT, comm);
#endif

#ifdef ALLGATHER
	void *sendBuf, *recvBuf;
	sendBuf = malloc (dataTypeSize * count);
	recvBuf = malloc (dataTypeSize * count * size);

	//warmup phase
	for (i = 0; i < 512; i++)
		allGather (sendBuf, count, datatype, recvBuf, count, datatype, comm);
#endif

#ifdef ALLTOALL
	void *sendBuf, *recvBuf;
	sendBuf = malloc (dataTypeSize * count * size);//use a generic function.
	recvBuf = malloc (dataTypeSize * count * size);
	
	//warmup phase
	for (i = 0; i < 512; i++)
		alltoall (sendBuf, count, datatype, recvBuf, count, datatype, comm);
#endif

	minMaxAverage = 0;
	localAverage = 0;

	for (i = 0; i < NOOFREADINGS; i++)
	{
		MPI_Barrier (comm);
		start = getTime ();

#ifdef ALLREDUCE
		allReduce (sendBuf, recvBuf, count, FLOAT, SUM, comm);
#endif

#ifdef BARRIER
		barrier (comm);
#endif

#ifdef BROADCAST
		broadcast (buffer, count, datatype, ROOT, comm);
#endif
	
#ifdef GATHER
		gather (sendBuf, count, datatype, recvBuf, count, datatype, ROOT, comm);
#endif

#ifdef ALLGATHER
		allGather (sendBuf, count, datatype, recvBuf, count, datatype, comm);
#endif
		
#ifdef ALLTOALL
		alltoall (sendBuf, count, datatype, recvBuf, count, datatype, comm);
#endif	
		
		end = getTime ();
		
		//Adjust the readings for skew here.
		start -= skew [rank];  
		end -= skew [rank];
				
		//Get min start and max end time.	
		callReduce (&start, &minStart, 1, DOUBLE, MIN, ROOT, comm);
		callReduce (&end, &maxEnd, 1, DOUBLE, MAX, ROOT, comm);
		
		//Multiple timing values are taken. Process them on rank 0.
		//Every time a newreading is obtained. Update max and min values
		//and update sum for average. 
		if (rank == 0)
		{
			diff = (maxEnd - minStart) * 1e6;
			minMaxAverage += diff;
			allDiffs [i] = diff;

			if (i == 0)
			{
				min = diff;
				max = diff;
			}

			else
			{
				if (min > diff)
					min = diff;
				
				if (max < diff)
					max = diff;
			}

			
		}
		
		MPI_Barrier (comm);

	}

	minMaxAverage = minMaxAverage / NOOFREADINGS;
	
	for (i = 0; i < NOOFREADINGS; i++)
		stdDev += (allDiffs [i] - minMaxAverage) * (allDiffs [i] - minMaxAverage);

	stdDev = sqrt (stdDev / NOOFREADINGS);
	callReduce (&localAverage, &globalAverage, 1, DOUBLE, SUM, ROOT, comm);

	if (rank == ROOT)
	//Output format adapted from osu benchmark.
	//http://mvapich.cse.ohio-state.edu/benchmarks
		printf ("%i\t%i\t%f\t%f\t%f\t%f\n", count, size, minMaxAverage, min, max, stdDev);
	
#ifdef ALLREDUCE
	free (sendBuf);
	free (recvBuf);
#endif

#ifdef BROADCAST
	free (buffer);
#endif

#ifdef GATHER
	free (sendBuf);
	free (recvBuf);
#endif
	
#ifdef ALLGATHER
	free (sendBuf);
	free (recvBuf);
#endif

#ifdef ALLTOALL
	free (sendBuf);
	free (recvBuf);
#endif
}

//Timing routine is benched mark in this function.
double benchMarkTimingRoutine ()
{
	double start, end, temp;
	int i;
	const int reps = 1000;

	start = getTime ();
	
	for (i = 0; i < reps; i++)
		getTime ();

	end = getTime ();

	return (end - start) / reps;
}

int main (int argc, char *argv [])
{
	COMMTYPE comm, newComm;
	int rank, size, temp;
	int i, j;
	long WTimeFlag;
	double *skew, exTime;
	
	init (argc, argv, &WTimeFlag, &rank, &size, &comm);
	
	callBarrier (comm);


	//Benchmark timing routine.
	exTime = benchMarkTimingRoutine ();	

	skew = (double *) malloc (sizeof (double) * size);
	//Root rank calculate skew between all ranks.
	getClkSkew (skew, comm);

	callBarrier (comm);

	//Distribute skew among all ranks
	callBroadcast (skew, sizeof (double) * size, BYTE, ROOT, comm);
	
	callBarrier (comm);

#ifdef DEBUG
	for (i = 0; i < size; i++)
	{
		printf ("rank: %i skew [%i] = %f\n", rank, i, skew [i]);
	}
#endif

	if (rank == 0)
	{	
		//Output format adapted from osu benchmark.
		//http://mvapich.cse.ohio-state.edu/benchmarks/osu-micro-benchmarks-3.9.tar.gz
		printf ("Data\tranks\taverage (us)\tmin (us)\tmax (us) \tstdDev (us)\n");
	}

#ifndef SKEWANDLATENCY	
	//i indicates info about no of ranks present inside a communicator. 	
	for (i = atoi (argv [1]); i <= size; i = i << 1)//comm spliting loop.
	{
		callBarrier (comm);

		//j represents here message size.
		for (j = atoi (argv [2]); j <= atoi (argv [3]); j = j << 1) //data size increased loop.  
		{
			//Make a new communicator here.
			if (callCommSplit (comm, (rank < i), rank, &newComm) != MPI_SUCCESS)
			{
				printf ("Comm split failed");

				goto exit;
			}		

			if (rank < i)
			{			

				timeCollective (skew, j, newComm, FLOAT, atoi (argv [4]),atoi (argv [5]));
				
				//destory communicator here
				callCommFree (&newComm);
			}
		}
	}
#endif
	
	free (skew);	

exit:	callFinalize();
	return 0;
}
