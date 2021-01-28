SHELL=bash
GCC=g++ -std=c++11 -Wall -Wextra -Werror -Wno-sign-compare -O2 

.PHONY: parcel parcel2 cut sac

clean:
	rm -f *.o *.h.gch *.test a.out

parcel.o: parcel.h parcel.cc
	$(GCC) -c parcel.cc -o parcel.o

parcel.test: parcel.o main_utils.h parcel.test.cc
	$(GCC) parcel.o parcel.test.cc -o parcel.test

parcel: parcel.test
	./parcel.test

parcel2.o: parcel2.h parcel2.cc
	$(GCC) -c parcel2.cc -o parcel2.o

parcel2.test: parcel2.o main_utils.h parcel2.test.cc
	$(GCC)  parcel2.o parcel2.test.cc -o parcel2.test

parcel2: parcel2.test
	./parcel2.test

cut.o: cut.h cut.cc
	$(GCC) -c cut.cc -o cut.o

cut.test: cut.o main_utils.h cut.test.cc
	$(GCC) cut.o cut.test.cc -o cut.test

cut: cut.test
	./cut.test

sac.o: sac.h sac.cc
	$(GCC) -c sac.cc -o sac.o

sac.test: sac.o main_utils.h sac.test.cc
	$(GCC) sac.o sac.test.cc -o sac.test

sac: sac.test
	./sac.test
