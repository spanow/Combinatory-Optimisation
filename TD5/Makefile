SHELL=bash
GCC=g++ -std=c++11 -Wall -Wextra -Werror -Wno-sign-compare -O2 

.PHONY: sac1 sac2 prim sac

clean:
	rm -f *.o *.h.gch *.test a.out

sac1.o: sac1.h sac1.cc
	$(GCC) -c sac1.cc -o sac1.o

sac1.test: sac1.o main_utils.h sac1.test.cc
	$(GCC) sac1.o sac1.test.cc -o sac1.test

sac1: sac1.test
	./sac1.test

sac2.o: sac2.h sac2.cc
	$(GCC) -c sac2.cc -o sac2.o

sac2.test: sac2.o main_utils.h sac2.test.cc
	$(GCC)  sac2.o sac2.test.cc -o sac2.test

sac2: sac2.test
	./sac2.test

prim.o: prim.h prim.cc
	$(GCC) -c prim.cc -o prim.o

prim.test: graph.o ugraph.o prim.o main_utils.h graph_utils.h prim.test.cc
	$(GCC) graph.o ugraph.o prim.o prim.test.cc -o prim.test

prim: prim.test
	./prim.test

graph.o: graph.h graph.cc
	$(GCC) -c graph.cc -o graph.o

color.o: color.h color.cc
	$(GCC) -c color.cc -o color.o

ugraph.o: graph.h ugraph.h ugraph.cc
	$(GCC) -c ugraph.cc -o ugraph.o

color.test: graph.o ugraph.o color.o main_utils.h graph_utils.h color.test.cc
	$(GCC) graph.o ugraph.o color.o color.test.cc -o color.test

color: color.test
	./color.test
