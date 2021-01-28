SHELL=bash
GCC=g++ -std=c++11 -Wall -Wextra -Werror -Wno-sign-compare -O2 

.PHONY: tree.1 tree.2 tree.3 vgraph dijkstra

clean:
	rm -f *.o *.h.gch *.test a.out

tree.1.o: tree.1.h tree.1.cc
	$(GCC) -c tree.1.cc -o tree.1.o

tree.1.test: tree.1.o main_utils.h tree.1.test.cc
	$(GCC) tree.1.o tree.1.test.cc -o tree.1.test

tree.1: tree.1.test
	./tree.1.test

tree.2.o: tree.2.h tree.2.cc
	$(GCC) -c tree.2.cc -o tree.2.o

tree.2.test: tree.2.o main_utils.h tree.2.test.cc
	$(GCC)  tree.2.o tree.2.test.cc -o tree.2.test

tree.2: tree.2.test
	./tree.2.test

tree.3.o: tree.3.h tree.3.cc
	$(GCC) -c tree.3.cc -o tree.3.o

tree.3.test: tree.3.o main_utils.h tree.3.test.cc
	$(GCC) tree.3.o tree.3.test.cc -o tree.3.test

tree.3: tree.3.test
	./tree.3.test

tree.4.o: tree.4.h tree.4.cc
	$(GCC) -c tree.4.cc -o tree.4.o

tree.4.test: tree.4.o main_utils.h tree.4.test.cc
	$(GCC) tree.4.o tree.4.test.cc -o tree.4.test

tree.4: tree.4.test
	./tree.4.test

graph.o: graph.h graph.cc
	$(GCC) -c graph.cc -o graph.o

toposort.o: graph.h toposort.h toposort.cc
	$(GCC) -c toposort.cc -o toposort.o

toposort.test: graph.o toposort.o main_utils.h toposort.test.cc
	$(GCC) graph.o toposort.o toposort.test.cc -o toposort.test

toposort: toposort.test
	./toposort.test

longest_path_dag.o: graph.h longest_path_dag.h longest_path_dag.cc
	$(GCC) -c longest_path_dag.cc -o longest_path_dag.o

longest_path_dag.test: graph.o longest_path_dag.o main_utils.h longest_path_dag.test.cc
	[[ -f "toposort.h" ]] && $(GCC) graph.o longest_path_dag.o toposort.cc longest_path_dag.test.cc -o longest_path_dag.test || $(GCC) graph.o longest_path_dag.o longest_path_dag.test.cc -o longest_path_dag.test

longest_path_dag: longest_path_dag.test
	./longest_path_dag.test
