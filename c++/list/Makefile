# Makefile for Project 5
#****************************************************************

PROG = anglosaxon
CPPOPTIONS = -I../StanfordCPPLib -fvisibility-inlines-hidden -std=c++11
LDOPTIONS = -L../StanfordCPPLib
LIB = -lStanfordCPPLib

all: test_list

test_list: test_list.o list.o
	g++ $(LDOPTIONS) -o test_list test_list.o list.o $(LIB)

test_list.o: test_list.cpp list.h
	g++ $(CPPOPTIONS) -c test_list.cpp

$(PROG): $(PROG).o list.o
	g++ $(LDOPTIONS) -o $(PROG) $(PROG).o list.o $(LIB)

$(PROG).o: $(PROG).cpp list.h
	g++ $(CPPOPTIONS) -c $(PROG).cpp

list.o: list.cpp list.h
	g++ $(CPPOPTIONS) -c list.cpp