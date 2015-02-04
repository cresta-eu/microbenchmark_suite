#Valid options: allReduce, barrier, broadcast, gather, allGather, alltoall
paradigm=MPI

CC=cc
CFLAGS=-I. -g -O0
LDFLAGS=
XX=aprun

OBJECTS=mpiWrapper.o benchmark.o timing.o $(coll).o

collLib=impl/$(coll).h
collImpl=impl/$(coll).c

#Variable used to control which ranks needs to be delayed.
delayRank=0
delay=0   #in micro seconds

#process start and end limits
startNo=2
endNo=32

#data start and end limits
dataStart=512
dataEnd=2048

$(coll).o:	$(collImpl)
	$(CC) $(CFLAGS) -c $< -o $@ -D$(paradigm)	

mpiWrapper.o:	mpiWrapper.c
	$(CC) $(CFLAGS) -c $< -o $@  

benchmark.o:	benchmark.c
	$(CC) $(CFLAGS) -c $< -o $@ -D$(coll)=$(coll) -DcollLib=$(collLib) -D$(paradigm)

timing.o:	timing.c
	$(CC) $(CFLAGS) -c $< -o $@
	
mbs_$(coll).out:	$(OBJECTS)
	$(CC) $(CFLAGS) -o mbs_$(coll).out $(OBJECTS)

all:	mbs_$(coll).out

run:	all
	$(XX) -n $(endNo) mbs_$(coll).out $(startNo) $(dataStart) $(dataEnd) $(delayRank) $(delay)

clean:
	rm -f benchmark
	rm -f *.o *.out
