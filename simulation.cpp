// Epidemic simulation

#include <stdio.h>

#include "Disease.h"
#include "Agent.h"
#include "Epidemic.h"

using namespace std;

int main() {

  // =====================================================================
  // SIMULATION PARAMETERS

  const int num_agents = 1e6;
  const int num_initially_infected = 1000;
  const int base_encounters = 10;

  const int num_iterations = 0;
  bool verbose = true;

  char states_fname[] = "states.txt";

  const bool full_dump = false;

  // =====================================================================
  // DISEASE

  string name = "COVID19 variant 1";
  double transm_prob = 0.10;
  // double transm_prob = 0.10;
  double latency_period = 2;
  double incubation_period = 3;
  double symptoms_duration = 5;
  double contagious_duration = (incubation_period + symptoms_duration) - latency_period;
  double max_severity = 2;
  double fatality_rate = 0.01;
  Disease* variant1 = new Disease(name, transm_prob, latency_period, contagious_duration, incubation_period, symptoms_duration, max_severity, fatality_rate);

  // =====================================================================
  // Run simulation

  Epidemic* ep = new Epidemic(num_agents, num_initially_infected, base_encounters, states_fname, full_dump);

  ep->initialize();
  ep->random_infect(num_initially_infected, variant1);

  ep->run_simulation(num_iterations, verbose);
  
  int tot_infected = num_agents - ep->states_counts[STATE_HEALTHY];
  double pct_infected = 100*(double)tot_infected/num_agents;
  printf("Population infected: %i : %.1f%%\n", tot_infected, pct_infected);

}