// Epidemic simulation

#include <stdio.h>

#include "Disease.h"
#include "Agent.h"
#include "Epidemic.h"

using namespace std;

int main() {

  // =====================================================================
  // GENERAL SIMULATION PARAMETERS

  const int num_agents = 1e6;
  const int num_initially_infected = 1000;
  const int base_encounters = 10;

  const int num_iterations = 0;
  bool verbose = true;

  char states_fname[] = "states.txt";

  const bool full_dump = false;

  // =====================================================================
  // PARAMETER RANGES

  const double transm_min = 0.01;
  const double transm_max = 0.08;
  const double transm_step = 0.01;

  const double severity_min = 1;
  const double severity_max = 10;
  const double severity_step = 1;

  const int num_transm = int((transm_max - transm_min)/transm_step + 1);
  const int num_sever = int((severity_max - severity_min)/severity_step + 1);
  const int num_variants = num_transm * num_sever;

  // Common variant parameters

  const double latency_period = 2;
  const double incubation_period = 3;
  const double symptoms_duration = 5;
  const double contagious_duration = (incubation_period + symptoms_duration) - latency_period;
  const double fatality_rate = 0.01;

  // =====================================================================
  // Run simulations

  Epidemic* ep;
  Disease* variant;
  char variant_name[100];
  double transm, sever;
 
  for (int i = 0; i < num_transm; i++) {
    
    transm = transm_min + transm_step*i;

    for (int j = 0; j < num_sever; j++) {
    
      sever = severity_min + severity_step*j;

      printf("transm=%.3f, sever=%.0f : ", transm, sever);

      sprintf(variant_name, "COVID19 variant transm=%.3f sever=%.0f", transm, sever);
      variant = new Disease(0, variant_name, transm, latency_period, contagious_duration, incubation_period, symptoms_duration, sever, fatality_rate);
      std::vector<Disease*> diseases;
      diseases.push_back(variant);

      ep = new Epidemic(num_agents, diseases, base_encounters, states_fname, full_dump);

      ep->initialize();
      ep->random_infect(num_initially_infected, variant);

      ep->run_simulation(num_iterations, false);

      int tot_infected = num_agents - ep->states_counts[STATE_HEALTHY];
      double pct_infected = 100*(double)tot_infected/num_agents;
      printf("%.1f%%\n", pct_infected);

      FILE* output_file = fopen("landscape.txt", "a");
      fprintf(output_file, "%f %f %f\n", transm, sever, pct_infected);
      fclose(output_file);

      delete ep;
      

    }

  }



}