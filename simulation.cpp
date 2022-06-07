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
  const int num_variants = 1;
  const int num_initially_infected = 1000;
  const int base_encounters = 10;

  const int num_iterations = 0;
  bool verbose = true;

  char states_fname[] = "";

  const bool full_dump = false;

  // =====================================================================
  // Create disease

  string name = "COVID19";
  double transm_prob = 0.10;
  // double transm_prob = 0.10;
  double latency_period = 2;
  double incubation_period = 3;
  double symptoms_duration = 5;
  double contagious_duration = (incubation_period + symptoms_duration) - latency_period;
  double max_severity = 2;
  double fatality_rate = 0.01;
  Disease* disease = new Disease(0, name, transm_prob, latency_period, contagious_duration, incubation_period, symptoms_duration, max_severity, fatality_rate);

  std::vector<Disease*> diseases;
  diseases.push_back(disease);

  // =====================================================================
  // Run simulation

  Epidemic* ep = new Epidemic(num_agents, diseases, base_encounters, states_fname, full_dump);

  ep->initialize();
  
  ep->random_infect(num_initially_infected, disease); 

  ep->run_simulation(num_iterations, verbose);
  
  int tot_cumul_infected = 0;
  for (int i = 0; i < diseases.size(); i++) {
    tot_cumul_infected += disease->cumul_infected;
    printf("%s: %i = %.1f%%\n", diseases[i]->name.c_str(), diseases[i]->cumul_infected, 100.0*diseases[i]->cumul_infected/num_agents);
  }


}