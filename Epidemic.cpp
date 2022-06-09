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

#include "Disease.h"
#include "Epidemic.h"

// ---------------------------------------------------------------------

// Default constructor
Epidemic::Epidemic(int _num_agents, std::vector<Disease*>& _diseases, int _base_encounters, char* _states_fname, bool _full_output) {

  num_agents = _num_agents;
  diseases = _diseases;
  num_diseases = diseases.size();
  base_encounters = _base_encounters;
  states_fname = _states_fname;
  full_output = _full_output;

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

  // Reset state counts
  states_counts = new int[num_states];
  for (int i = 0; i < num_states; i++) {
    states_counts[i] = 0;
  }

  // Reset disease counts
  for (int i = 0; i < diseases.size(); i++) {
    diseases[i]->cumul_infected = 0;
    diseases[i]->curr_infected = 0;
  }

  // Create agents
  agents.clear();
  Agent* agent;
  for (int count = 1; count <= num_agents; count++) {
    agent = new Agent(count, base_encounters);
    agents.push_back(agent);
  }

  // Open new output file, if given
  if ((states_fname != NULL) && (states_fname[0] != '\0')) {
    
    output_file = fopen(states_fname, "w");
    if (output_file != NULL) {
      do_output = true;
      fclose(output_file);
      // printf("Writing to %s\n", states_fname);
    } else {
      do_output = false;
      printf("Couldn't open file %s; skipping output\n", states_fname);
    }

  } else {

    do_output = false;
    printf("No output file given\n");

  }


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
void Epidemic::update_counts() {

  for (int i = 0; i < num_states; i++) states_counts[i] = 0;
  for (int i = 0; i < diseases.size(); i++) diseases[i]->curr_infected = 0;

  for (int i = 0; i < (int)agents.size(); i++) {
    states_counts[agents[i]->state] += 1;
    if (agents[i]->isInfected) {
      agents[i]->disease->curr_infected += 1; 
    }
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

// Report diseases counts
void Epidemic::report_diseases() {

  for (int i = 0; i < num_diseases; i++) {
    printf("%s: cur %i, cumul %i\n", diseases[i]->name.c_str(), diseases[i]->curr_infected, diseases[i]->cumul_infected);
  }

}

// ---------------------------------------------------------------------

// Writes currents states to the output file
// if full_dump, writes the states of all agents; if not, only counts
void Epidemic::output_states() {

  if (!do_output) return;

  output_file = fopen(states_fname, "a");
  
  if (full_output) {
    
    // Write full agent states
    for (int i = 0; i < num_agents; i++) {
      fprintf(output_file, "%i", agents[i]->state);
      if (i < num_agents-1) fprintf(output_file, " ");
    }
    fprintf(output_file, "\n");

  } else {
    
    // Write only total counts:
    // healthy tot_infected recovered dead diseases_counts cumul_dis_counts
    fprintf(output_file, "%i", states_counts[STATE_HEALTHY]);
    fprintf(output_file, " %i", states_counts[STATE_INFECTED]);
    fprintf(output_file, " %i", states_counts[STATE_RECOVERED]);
    fprintf(output_file, " %i", states_counts[STATE_DEAD]);
    for (int v = 0; v < diseases.size(); v++) {
      fprintf(output_file, " %i", diseases[v]->curr_infected);
    }
    for (int v = 0; v < diseases.size(); v++) {
      fprintf(output_file, " %i", diseases[v]->cumul_infected);
    }    
    fprintf(output_file, "\n");

  }
  
  fclose(output_file);

}

// ---------------------------------------------------------------------

// Makes have random encounters between each other
void Epidemic::socialize_agents() {

  int id, other_id, num_encounters;
  double prob;
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
          prob = agent->disease->get_transm_prob(agent->days_infected);
          if (randreal() <= prob) {
            other->infect(agent->disease);
            other->disease->cumul_infected += 1;
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
// Assumes agents have already been created and infected
void Epidemic::run_simulation(int total_iterations, bool verbose) {

  update_counts();
  if (do_output) output_states();
  
  if (verbose) {
    printf("----------\nstart\n");
    report_states();
  }

  iteration = 1;
  while (true) {

    if (verbose) printf("----------\nday = %i\n", iteration);

    // Make agents socialize (and spread disease)
    socialize_agents();

    // Evolve disease of all agents
    evolve_diseases();

    // Update counts
    update_counts();
    
    if (do_output) output_states();
    if (verbose) report_states();

    iteration++;
    if (total_iterations == 0) {
      if (states_counts[STATE_INFECTED] == 0) break;
    } else {
      if (iteration == total_iterations) break;
    }

  }

  if (verbose) printf("----------\nSimulation completed\n");


}
 