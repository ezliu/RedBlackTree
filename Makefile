GTEST_DIR=/Users/Evan/Documents/code/googletest/googletest
GCC=g++
CXXFLAGS=-g -Wall -I$(INCLUDE)
INCLUDE=./inc
OBJECTS=./obj

VPATH=${INCLUDE}

all: getobj myTests ctags putobj

.PHONY: getobj
getobj:
	-mv ${OBJECTS}/*.[oa] . 2>/dev/null

.PHONY: putobj
putobj:
	-mv *.[oa] ${OBJECTS} 2>/dev/null

myTests: myTests.o 
	${GCC} ${CXXFLAGS} -isystem ${GTEST_DIR}/include myTests.o ${GTEST_DIR}/libgtest.a -o myTests

myTests.o: myTests.cpp RedBlackTree.h
	${GCC} ${CXXFLAGS} -I${GTEST_DIR}/include -c myTests.cpp

gtest:
	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

.PHONY: clean
clean:
	-rm -f myTests
	-rm -f ${OBJECTS}/*.[oa]

.PHONY: ctags
ctags:
	-ctags -R *

rebuild: clean gtest all

