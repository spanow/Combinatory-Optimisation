SHELL=bash
GCC=g++ -std=c++11 -Wall -Wextra -Werror -Wno-sign-compare -O2 -Wno-deprecated -I../include -DUSE_CBC -DUSE_GLOP -DUSE_BOP -Wno-unused-parameter
ORTOOLS=../lib/libortools.so
ORLNK=$(ORTOOLS)
OS = $(shell uname -s)

ifeq ($(OS),Darwin) # Assume Mac Os X
	ORTOOLS=../lib/libortools.dylib
        ORLNK=-Wl,-rpath ../lib -L../lib -lortools -lz
endif

.PHONY: job flow2

clean:
	rm -f *.o *.h.gch *.test a.out

%.o: %.cc %.h
	$(GCC) -c $< -o $@

flow2.test: flow2.o $(ORTOOLS) main_utils.h flow2.test.cc
	$(GCC) flow2.o $(ORLNK) flow2.test.cc -o $@

flow2: flow2.test
	./flow2.test

job.test: job.o $(ORTOOLS) main_utils.h job.test.cc
	$(GCC) job.o $(ORLNK) job.test.cc -o $@

job: job.test
	./job.test

