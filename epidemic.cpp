// Epidemic simulation
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "Disease.h"
#include "Agent.h"

// =====================================================================
// PROGRAM PARAMETERS

const int num_agents = 1000000;
const int base_encounters = 10;

const int num_initially_infected = 100;

const bool simulate_until_no_infected = true;
const int num_iterations = 60;

char states_fname[] = "states.txt";

const bool full_dump = false;

// =====================================================================
// DISEASES

string name = "COVID19";
double transm_prob = 0.05;
double duration = 7;
double contagious_onset = 2;
double symptoms_onset = 3;
double severity = 5;
double fatality_rate = 0.001;
Disease* disease1 = new Disease(name, transm_prob, duration, contagious_onset, symptoms_onset, severity, fatality_rate);

// =====================================================================
// GLOBALS

// List of agents
std::vector<Agent*> agents;

int* states_counts;
FILE* output_file;
int iteration;
unsigned int global_seed;

// =====================================================================
// FUNCTIONS

// ---------------------------------------------------------------------

// Sets the seed of the RNG
void set_seed(unsigned int seed) {
  srand(seed);
  global_seed = seed;
}

// Resets the seed of the RNG (to a new random seed)
void reset_seed() {
  srand(time(NULL));
  set_seed(rand());
}

// ---------------------------------------------------------------------

void initialize() {

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

void infect_init(int num) {

  int id;
  int num_infected = 0;
  while (num_infected < num) {
    id = randint(num_agents);
    if (!agents[id]->isInfected) {
      agents[id]->infect(disease1);
      num_infected++;
    }
  }

}

// ---------------------------------------------------------------------

void count_states() {

  int i;
  for (i = 0; i < num_states; i++) states_counts[i] = 0;
  for (i = 0; i < (int)agents.size(); i++) {
    states_counts[agents[i]->state] += 1;
  }

}

// ---------------------------------------------------------------------

void print_states() {

  for (int i = 0; i < num_states; i++) {
    printf("%s: %i (%.1f%%)\n", states_names[i].c_str(), states_counts[i], 100.0*states_counts[i]/num_agents);
  }

}

// ---------------------------------------------------------------------

void output_states() {

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

void socialize_agents() {

  int id, other_id, encounters;
  Agent* agent;
  Agent* other;
  
  for (int id = 0; id < num_agents; id++) {
    agent = agents[id];
    if ((agent->isAlive) && (agent->isContagious)) { 

      for (int i = 0; i <= agent->base_encounters; i++) {

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

void evolve_diseases() {

  for (int i=0; i < num_agents; i++) {
    if ((agents[i]->isAlive) && (agents[i]->isInfected)) {
      agents[i]->evolve_disease();
    }
  }

}

// =====================================================================

int main (int argc, char** argv) {

  initialize();

  infect_init(num_initially_infected);

  count_states(); print_states();
  output_states();

  iteration = 1;
  while (true) {

    printf("----------\nit = %i\n", iteration);

    // Make agents socialize (and spread disease)
    socialize_agents();

    // Evolve disease of all agents
    evolve_diseases();

    // Update counts
    count_states();
    print_states();
    output_states();

    iteration++;
    if (simulate_until_no_infected) {
      if (states_counts[STATE_INFECTED] == 0) break;
    } else {
      if (iteration == num_iterations) break;
    }

  }

  return 0;

}