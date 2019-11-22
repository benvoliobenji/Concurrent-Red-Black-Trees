CC = gcc
CXX = g++
ECHO = echo
RM = rm -f

CFLAGS = -Wall -Werror -ggdb -funroll-loops
CXXFLAGS = -std=c++14 -Wall -g

BIN = RBTree
OBJS = RBTreeRun.o RBTree.o Node.o Parser.o trim.o

all: RBTreeRun.cpp
		@$(ECHO) Compiling RBTree
		g++ -std=c++14 -Wall -g RBTreeRun.cpp RBTree.cpp Node.cpp Parser.cpp trim.cpp -o RBTree

clean:
		@$(ECHO) Removing all generated files
		@$(RM) *.o $(BIN) *.d TAGS core vgcore.* gmon.out

clobber: clean
		@$(ECHO) Removing backup files
		@$(RM) *~ \#* *pgm

etags:
		@$(ECHO) Updating TAGS
		@etags *.[ch]