GTEST_DIR=/Users/Evan/Downloads/gtest-1.7.0
GCC=g++
CXXFLAGS=-g -Wall -I$(INCLUDE)
INCLUDE=./inc
OBJECTS=./obj

VPATH=${INCLUDE}

all: getobj myTests runtime putobj

getobj:
	-mv ${OBJECTS}/*.[oa] . 2>/dev/null

putobj:
	-mv *.[oa] ${OBJECTS} 2>/dev/null

myTests: myTests.o 
	${GCC} ${CXXFLAGS} -isystem ${GTEST_DIR}/include myTests.o libgtest.a -o myTests

myTests.o: myTests.cpp RedBlackTree.h
	${GCC} ${CXXFLAGS} -I${GTEST_DIR}/include -c myTests.cpp

runtime: runtime.o
	${GCC} ${CXXFLAGS} -o runtime runtime.o

gtest:
	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

clean:
	-rm -f myTests
	-rm -f ${OBJECTS}/*.[oa]

rebuild: clean gtest all

