SHELL=bash
GCC=g++ -std=c++11 -Wall -Wextra -Werror -Wno-sign-compare -O1
# AS: replaced -O2 by -O1 because g++-8 compiler optimizations makes one of 
# the tests in bfs.1 fail (specifically, the one with a for loop which adds
# N random edges: it actually will generate only one random number and add 
# N times the same edge

.PHONY: bfs.1 bfs.2 bfs.3 vgraph dijkstra

clean:
	rm -f *.o *.h.gch *.test a.out

graph.o: graph.h graph.cc
	$(GCC) -c graph.cc -o graph.o

ugraph.o: graph.h ugraph.h ugraph.cc
	$(GCC) -c ugraph.cc -o ugraph.o

bfs.1.o: graph.h ugraph.h bfs.1.h bfs.1.cc
	$(GCC) -c bfs.1.cc -o bfs.1.o

bfs.1.test: graph.o ugraph.o bfs.1.o main_utils.h bfs.1.test.cc
	$(GCC) graph.o ugraph.o bfs.1.o bfs.1.test.cc -o bfs.1.test

bfs.1: bfs.1.test
	./bfs.1.test

bfs.2.o: graph.h ugraph.h bfs.2.h bfs.2.cc
	$(GCC) -c bfs.2.cc -o bfs.2.o

bfs.2.test: graph.o ugraph.o bfs.2.o main_utils.h bfs.2.test.cc
	$(GCC) graph.o ugraph.o  bfs.2.o bfs.2.test.cc -o bfs.2.test

bfs.2: bfs.2.test
	./bfs.2.test

bfs.3.o: graph.h ugraph.h bfs.3.h bfs.3.cc
	$(GCC) -c bfs.3.cc -o bfs.3.o

bfs.3.test: graph.o ugraph.o bfs.3.o main_utils.h bfs.3.test.cc
	$(GCC) graph.o ugraph.o bfs.3.o bfs.3.test.cc -o bfs.3.test

bfs.3: bfs.3.test
	./bfs.3.test

vgraph.o: vgraph.h vgraph.cc
	$(GCC) -c vgraph.cc -o vgraph.o

vgraph.test: vgraph.o main_utils.h vgraph.test.cc
	$(GCC) vgraph.o vgraph.test.cc -o vgraph.test

vgraph: vgraph.test
	./vgraph.test

dijkstra.o: vgraph.h dijkstra.h dijkstra.cc
	$(GCC) -c dijkstra.cc -o dijkstra.o

dijkstra.test: vgraph.o dijkstra.o main_utils.h dijkstra.test.cc
	$(GCC) vgraph.o dijkstra.o dijkstra.test.cc -o dijkstra.test

dijkstra: dijkstra.test
	./dijkstra.test
