//==============================================================================
// Epidemic class declaration
//==============================================================================
#ifndef EPIDEMIC_H
#define EPIDEMIC_H

#include <vector>
#include "Disease.h"
#include "Agent.h"

class Epidemic {
  
  public:
  
  // -----------------------------------------------------------------------
  // CLASS VARIABLES

  // Simulation parameters

  int num_agents;                // Number of agents in the simulation
  int num_diseases;              // Number of diseases in the simulation
  int base_encounters;           // Base number of encounters for the agents

  char* states_fname;            // Filename for the states
  bool full_dump = false;        // Dump states of all agents, or just counts?

  // Class globals

  int* states_counts;                // Counts of current agents states
  std::vector<Disease*> diseases;    // List of Diseases
  std::vector<Agent*> agents;        // List of Agents

  int iteration;                 // Current iteration
  unsigned int seed;             // RNG seed currently in use
  
  bool do_output;                // Do outputs?
  FILE* output_file;             // Handle for the output file

  // -----------------------------------------------------------------------
  // CLASS METHODS

  // Default constructor
  Epidemic(int _num_agents, std::vector<Disease*>& _diseases, int _base_encounters, char* _states_fname, bool _full_dump);

  // Sets the seed of the RNG
  void set_seed(unsigned int seed);
  
  // Resets the seed of the RNG (to a new random seed)
  void reset_seed();  
  
  // Initialize the simulation
  void initialize();
  
  // Randomly infect agents
  void random_infect(int num, Disease* disease);
   
  // Update counts of agent states, variants, etc
  void update_counts();
  
  // Print state counts
  void report_states();
  
  // Write states to file
  void output_states();
  
  // Agent interactions
  void socialize_agents();
  
  // Evolve agent diseases
  void evolve_diseases();
  
  // Run a simulation
  void run_simulation(int total_iterations, bool verbose);

};

#endif