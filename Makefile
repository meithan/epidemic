# Compiler
COMPILER= g++

# User compiler flags
#USER_FLAGS = -g -Wall -pedantic
USER_FLAGS= -O3

# ==============================================================================

LDFLAGS= 
CCFLAGS=$(USER_FLAGS)
PROGRAMS=epidemic

# ==============================================================================

epidemic : epidemic.o
	$(COMPILER) $(LDFLAGS) epidemic.o -o epidemic

epidemic.o : Agent.h Disease.h epidemic.cpp

clean :
	rm -f *.o $(PROGRAMS)