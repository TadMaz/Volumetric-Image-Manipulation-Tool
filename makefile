# include dependencies; rules below
ifeq ($(UNAME),incl.defs)
	include incl.defs
endif

# Directories
SRC=./src
BIN=./bin

CC=g++
CCFLAGS=-std=c++11
SOURCES= volimage.cpp #StudentRecord.cpp
OBJECTS= volimage.o #StudentRecord.o

main: $(OBJECTS)
	$(CC) $(CCFLAGS) $(OBJECTS) -o volimage $(LIBS)

.cpp.o:
	$(CC) $(CCFLAGS) -c $<

# Type "make depend" to make dependencies
depend:
	$(CC) -M $(SOURCES) > incl.defs

run:
	./volimage

clean:
	rm *.o
	rm main
	rm incl.defs
	rm *.tar.gz
tar:
	tar -zcvf MZRTAD001.tar.gz makefile README.txt *.cpp *.h .git .gitignore