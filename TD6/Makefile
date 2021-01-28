SHELL=bash
GCC=g++ -std=c++11 -Wall -Wextra -Werror -Wno-sign-compare -O2 -Wno-deprecated

.PHONY: diam1 diam2 fas tsp

clean:
	rm -f *.o *.h.gch *.test a.out

%.o: %.cc %.h
	$(GCC) -c $< -o $@

diam1.test: diam1.o graph.o ugraph.o bfs.1.o bfs.2.o bfs.3.o main_utils.h graph_utils.h diam1.test.cc
	$(GCC) diam1.o graph.o ugraph.o bfs.1.o bfs.2.o bfs.3.o diam1.test.cc -o $@

diam1: diam1.test
	./diam1.test

diam2.test: diam2.o graph.o ugraph.o bfs.1.o bfs.2.o bfs.3.o main_utils.h graph_utils.h diam2.test.cc
	$(GCC) diam2.o graph.o ugraph.o bfs.1.o bfs.2.o bfs.3.o diam2.test.cc -o $@ || $(GCC) diam2.o diam1.o graph.o ugraph.o bfs.1.o bfs.2.o bfs.3.o diam2.test.cc -o $@ 

diam2: diam2.test
	./diam2.test

fas.test: fas.o ugraph.o graph.o toposort.o main_utils.h graph_utils.h fas.test.cc
	$(GCC) fas.o ugraph.o graph.o toposort.o fas.test.cc -o $@

fas: fas.test
	./fas.test

tsp.test: tsp.o main_utils.h tsp.test.cc
	$(GCC) tsp.o tsp.test.cc -o $@

tsp: tsp.test
	./tsp.test
