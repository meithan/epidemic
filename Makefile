# Compiler
COMPILER= g++

# User compiler flags
#USER_FLAGS = -g -Wall -pedantic
USER_FLAGS= -O3

# ==============================================================================

LDFLAGS= 
CCFLAGS= $(USER_FLAGS)
PROGRAMS= epidemic

# ==============================================================================

epidemic : Disease.h Agent.h epidemic.o
	$(COMPILER) $(LDFLAGS) epidemic.o -o epidemic

%.o : %.cpp
	$(COMPILER) $(CCFLAGS) -c -o $@ $<

%.h : ;

clean :
	rm -f *.o $(PROGRAMS)