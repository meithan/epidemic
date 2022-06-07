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
PROGRAMS=simulation landscape competition

# Object files
BASE_OBJECTS=utils.o Agent.o Disease.o Epidemic.o
SIMUL_OBJECTS=$(BASE_OBJECTS) simulation.o
LAND_OBJECTS=$(BASE_OBJECTS) landscape.o
COMPETITION_OBJECTS=$(BASE_OBJECTS) competition.o

# ==============================================================================

simulation : $(SIMUL_OBJECTS)
	$(COMPILER) $(LDFLAGS) $(SIMUL_OBJECTS) -o simulation

landscape : $(LAND_OBJECTS)
	$(COMPILER) $(LDFLAGS) $(LAND_OBJECTS) -o landscape

competition : $(COMPETITION_OBJECTS)
	$(COMPILER) $(LDFLAGS) $(COMPETITION_OBJECTS) -o competition

%.o : %.cpp 
	$(COMPILER) $(CCFLAGS) -c $< -o $@

clean :
	rm -f *.o $(PROGRAMS)
