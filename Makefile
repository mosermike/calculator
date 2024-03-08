##################################################
##		 MAKEFILE calc	 		##
##		 Mike Moser 			##
##################################################

TODOFILE  = .todo

PROJECT = calc

GCC = /usr/bin/g++

CFLAGS += -Wall -O2 -pipe -Weffc++ -flto=4 -std=c++20
CFLAGS_GDB = -Wall -Weffc++ -O0 -ggdb #-Werror

INCLUDE += -I./include -I./src #-I./math -I./math1

LDFLAGS =

INSTALL = 

SRC = $(wildcard ./src/*.cpp *.cpp )
OBJ = $(SRC:%.cpp=%.o)
DST = calc.x
HDR = $(wildcard ./include/*.hpp *.hpp )

target debug   : override CFLAGS = $(CFLAGS_GDB)


all: $(DST)
	@cat $(SRC) $(HDR) | grep TODO | wc -l > $(TODOFILE)
	@echo "\e[1;32m| Only $(shell cat ${TODOFILE}) TODOs! \e[0m"

clean:
	rm $(OBJ) $(DST)


install: $(OBJ)
	$(GCC) $(OBJ) $(CFLAGS) $(INCLUDE) $(LIBRARY) $(LDFLAGS) -o $(INSTALL)/$(PROJECT)

uninstall:
	
	@echo "| Delete $(OBJ) $(INSTALL)"
	
	rm $(OBJ) $(INSTALL)
	
debug: $(DST)

%.o: %.cpp
	$(GCC) $(INCLUDE) $(CFLAGS) -c -o $@ $<
	
%.x: $(OBJ)
	$(GCC) $(OBJ) $(CFLAGS) $(INCLUDE) $(LIBRARY) $(LDFLAGS) -o $(DST)
	