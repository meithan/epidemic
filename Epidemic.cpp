//==============================================================================
// Epidemic class implementation
//==============================================================================

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "Epidemic.h"

using namespace std;

// ---------------------------------------------------------------------

// Default constructor
Epidemic::Epidemic(int _num_agents, int _num_initially_infected, int _base_encounters, char* _states_fname, bool _full_dump) {

  num_agents = _num_agents;
  num_initially_infected = _num_initially_infected;
  base_encounters = _base_encounters;
  states_fname = _states_fname;

}

// ---------------------------------------------------------------------

// Sets the seed of the RNG
void Epidemic::set_seed(unsigned int seed) {
  srand(seed);
  this->seed = seed;
}

// Resets the seed of the RNG (to a new random seed)
void Epidemic::reset_seed() {
  srand(time(NULL));
  set_seed(rand());
}

// ---------------------------------------------------------------------

// Initializes a simulation:
// - Resets the RNG seed
// - Initializes states_counts
// - Creates agents
// - Opens output file
void Epidemic::initialize() {

  reset_seed();

  states_counts = new int[num_states];

  Agent* agent;
  for (int count = 1; count <= num_agents; count++) {
    agent = new Agent(count, base_encounters);
    agents.push_back(agent);
  }

  output_file = fopen(states_fname, "w");
  fclose(output_file);


}

// ---------------------------------------------------------------------

// Randomly infects 'num' agents with 'disease'
void Epidemic::random_infect(int num, Disease* disease) {

  int id;
  int num_infected = 0;
  while (num_infected < num) {
    id = randint(num_agents);
    if (!agents[id]->isInfected) {
      agents[id]->infect(disease);
      num_infected++;
    }
  }

}

// ---------------------------------------------------------------------

// Counts the number of agents in each state
void Epidemic::count_states() {

  int i;
  for (i = 0; i < num_states; i++) states_counts[i] = 0;
  for (i = 0; i < (int)agents.size(); i++) {
    states_counts[agents[i]->state] += 1;
  }

}

// ---------------------------------------------------------------------

// Report states counts to screen
void Epidemic::report_states() {

  for (int i = 0; i < num_states; i++) {
    printf("%s: %i (%.1f%%)\n", states_names[i].c_str(), states_counts[i], 100.0*states_counts[i]/num_agents);
  }

}

// ---------------------------------------------------------------------

// Writes currents states to the output file
// if full_dump, writes the states of all agents; if not, only counts
void Epidemic::output_states() {

  output_file = fopen(states_fname, "a");
  // fprintf(output_file, "%i ", iteration);
  
  if (full_dump) {
    
    // Write full agent states
    for (int i=0; i < num_agents; i++) {
      fprintf(output_file, "%i", agents[i]->state);
      if (i < num_agents-1) fprintf(output_file, " ");
    }
    fprintf(output_file, "\n");

  } else {
    
    // Write only total counts
    for (int i=0; i < num_states; i++) {
      fprintf(output_file, "%i", states_counts[i]);
      if (i < num_states-1) fprintf(output_file, " ");
    }
    fprintf(output_file, "\n");

  }
  
  fclose(output_file);

}

// ---------------------------------------------------------------------

// Makes have random encounters between each other
void Epidemic::socialize_agents() {

  int id, other_id, num_encounters;
  Agent* agent;
  Agent* other;
  
  for (int id = 0; id < num_agents; id++) {
    agent = agents[id];
    if ((agent->isAlive) && (agent->isContagious)) { 

      num_encounters = agent->get_encounters();

      for (int i = 1; i <= num_encounters; i++) {

        // Select another agent at random
        while (true) {
          other_id = randint(num_agents);
          if (other_id != id) break;
        }

        // If the other agent is healthy, possibly spread disease
        other = agents[other_id];
        if (other->state == STATE_HEALTHY) {
          if (randreal() <= agent->disease->transm_prob) {
            other->infect(agent->disease);
          }
        }

      }


    }
  }

}

// ---------------------------------------------------------------------

// Evolve diseases of all infected agents
void Epidemic::evolve_diseases() {

  for (int i=0; i < num_agents; i++) {
    if ((agents[i]->isAlive) && (agents[i]->isInfected)) {
      agents[i]->evolve_disease();
    }
  }

}

// =====================================================================

// Runs a full simulation
// Set total_iteration to zero to simulate until no infected left
void Epidemic::run_simulation(int total_iterations, bool verbose) {

  count_states();
  output_states();
  if (verbose) report_states();

  iteration = 1;
  while (true) {

    if (verbose) printf("----------\nday = %i\n", iteration);

    // Make agents socialize (and spread disease)
    socialize_agents();

    // Evolve disease of all agents
    evolve_diseases();

    // Update counts
    count_states();
    output_states();
    if (verbose) report_states();

    iteration++;
    if (total_iterations == 0) {
      if (states_counts[STATE_INFECTED] == 0) break;
    } else {
      if (iteration == total_iterations) break;
    }

  }

  if (verbose) printf("----------\n");

}
 