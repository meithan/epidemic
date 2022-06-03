# Compiler
COMPILER= g++

# User compiler flags
#USER_FLAGS = -g -Wall -pedantic
# USER_FLAGS= -O3 -Wall
USER_FLAGS= -O3

# Linker flags
LDFLAGS=

# ==============================================================================

CCFLAGS=$(USER_FLAGS)
PROGRAMS=simulation

# simulation
OBJECTS=utils.o Agent.o Disease.o Epidemic.o simulation.o

# ==============================================================================

simulation : $(OBJECTS)
	$(COMPILER) $(LDFLAGS) $(OBJECTS) -o simulation

%.o : %.cpp 
	$(COMPILER) $(CCFLAGS) -c $< -o $@

clean :
	rm -f $(OBJECTS) $(PROGRAMS)