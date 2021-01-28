SHELL=bash
GCC=g++ -std=c++11 -Wall -Wextra -Werror -Wno-sign-compare -O2

.PHONY: 1 2 3

clean:
	rm -f *.o *.h.gch *.test a.out

graph.o: graph.h graph.cc
	$(GCC) -c graph.cc -o graph.o

graph.1.test: graph.o main_utils.h graph.1.test.cc
	$(GCC) graph.o graph.1.test.cc -o graph.1.test

graph.2.test: graph.o main_utils.h graph.2.test.cc
	$(GCC) graph.o graph.2.test.cc -o graph.2.test

graph.3.test: graph.o main_utils.h graph.3.test.cc
	$(GCC) graph.o graph.3.test.cc -o graph.3.test

1: graph.1.test
	./graph.1.test

2: 1 graph.2.test
	./graph.2.test

3: 2 graph.3.test
	./graph.3.test

ugraph.o: graph.h ugraph.h ugraph.cc
	$(GCC) -c ugraph.cc -o ugraph.o

ugraph.1.test: graph.o ugraph.o main_utils.h ugraph.1.test.cc
	$(GCC) graph.o ugraph.o ugraph.1.test.cc -o ugraph.1.test

ugraph.2.test: graph.o ugraph.o main_utils.h ugraph.2.test.cc
	$(GCC) graph.o ugraph.o ugraph.2.test.cc -o ugraph.2.test

ugraph.3.test: graph.o ugraph.o main_utils.h ugraph.3.test.cc
	$(GCC) graph.o ugraph.o ugraph.3.test.cc -o ugraph.3.test

u1: ugraph.1.test
	./ugraph.1.test

u2: u1 ugraph.2.test
	./ugraph.2.test

u3: u2 ugraph.3.test
	./ugraph.3.test
