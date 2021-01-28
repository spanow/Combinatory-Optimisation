SHELL=bash
GCC=g++ -std=c++11 -Wall -Wextra -Werror -Wno-sign-compare -O2 -Wno-deprecated -I../include -DUSE_GLOP -Wno-unused-parameter
ORTOOLS=../lib/libortools.so

.PHONY: menu flow

clean:
	rm -f *.o *.h.gch *.test a.out

%.o: %.cc %.h
	$(GCC) -c $< -o $@

menu.test: menu.o $(ORTOOLS) main_utils.h menu.test.cc
	$(GCC) menu.o $(ORTOOLS) menu.test.cc -o $@

ortools_test.test: ortools_test.o $(ORTOOLS) main_utils.h ortools_test.test.cc
	$(GCC) ortools_test.o $(ORTOOLS) ortools_test.test.cc -o $@

ortools_test: ortools_test.test
	./ortools_test.test

menu: menu.test
	./menu.test

flow.test: flow.o $(ORTOOLS) main_utils.h flow.test.cc
	$(GCC) flow.o $(ORTOOLS) flow.test.cc -o $@

flow: flow.test
	./flow.test
