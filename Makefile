#################################
##		 MAKEFILE calc	 		#
#################################

PROJECT = calc

GCC = /usr/bin/g++

std = c++20

# Location where the log file is saved (in general in this directory to make sure that permissions are correct)
DVARS = -Drechner_log=\"$(PWD)/calc.log\"

CFLAGS += -Wall -O2 -pipe -Weffc++ -flto=4 -std=$(std)

CFLAGS_GDB = -Wall -Weffc++ -O0 -ggdb #-Werror

INCLUDE += -I./include -I./src

DST = calc.x

LIBRARY = -lmml -lmml-math

INSTALL_DIR = /usr/local/bin

SRC = $(wildcard *.cpp )
OBJ = $(SRC:%.cpp=%.o)

target debug   : override CFLAGS = $(CFLAGS_GDB)


all: $(DST)

clean:
	rm $(OBJ) $(DST)/calc.x


install: $(OBJ)
	$(GCC) $(OBJ) $(CFLAGS) $(INCLUDE) $(LIBRARY) $(LDFLAGS) -o $(INSTALL_DIR)/$(PROJECT)

uninstall:
	rm $(OBJ) $(DST)/$(PROJECT)

debug: $(DST)

%.o: %.cpp
	$(GCC) $(DVARS) $(INCLUDE) $(CFLAGS) -c -o $@ $<

%.x: $(OBJ)
	$(GCC) $(OBJ) $(CFLAGS) $(INCLUDE) $(LIBRARY) $(LDFLAGS) -o $(DST)

