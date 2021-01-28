SHELL=bash
GCC=g++ -std=c++11 -Wall -Wextra -Werror -Wno-sign-compare -O2 -Wno-deprecated -I../include -DUSE_CBC -DUSE_GLOP -DUSE_BOP -DUSE_SCIP -Wno-unused-parameter
ORTOOLS=../lib/libortools.so
LD_FLAGS = -Wl,-rpath,"../"  -Wl,-rpath,"../lib64" -Wl,-rpath,"../lib" -lz -lrt -lpthread
ORLNK=$(ORTOOLS) $(LD_FLAGS)
OS = $(shell uname -s)

ifeq ($(OS),Darwin) # Assume Mac Os X
	ORTOOLS=../lib/libortools.dylib
        ORLNK=-Wl,-rpath ../lib -L../lib -lortools -lz
endif

.PHONY: job1 job2 job3 job4 job0 students

clean:
	rm -f *.o *.h.gch *.test a.out

%.o: %.cc %.h
	$(GCC) -c $< -o $@

job1.test: job1.o $(ORTOOLS) main_utils.h job1.test.cc
	$(GCC) job1.o $(ORLNK) job1.test.cc -o $@

job1: job1.test
	./job1.test

job2.test: job2.o $(ORTOOLS) main_utils.h job2.test.cc
	$(GCC) job2.o $(ORLNK) job2.test.cc -o $@

job2: job2.test
	./job2.test

job3.test: job3.o $(ORTOOLS) main_utils.h job3.test.cc
	$(GCC) job3.o $(ORLNK) job3.test.cc -o $@

job3: job3.test
	./job3.test

job4.test: job4.o $(ORTOOLS) main_utils.h job4.test.cc
	$(GCC) job4.o $(ORLNK) job4.test.cc -o $@

job4: job4.test
	./job4.test

students.test: students.o $(ORTOOLS) main_utils.h students.test.cc
	$(GCC) students.o $(ORLNK) students.test.cc -o $@

students: students.test
	./students.test

job0.test: job0.o $(ORTOOLS) main_utils.h job0.test.cc
	$(GCC) job0.o $(ORLNK) job0.test.cc -o $@

job0: job0.test
	./job0.test
