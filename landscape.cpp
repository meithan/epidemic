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

  const double p_transm_min = 0.01;
  const double p_transm_max = 0.08;
  const double p_transm_step = 0.01;

  const double p_severity_min = 1;
  const double p_severity_max = 10;
  const double p_severity_step = 1;

  // =====================================================================
  // Run simulations

  const double latency_period = 2;
  const double incubation_period = 3;
  const double symptoms_duration = 5;
  const double contagious_duration = (incubation_period + symptoms_duration) - latency_period;
  const double max_dis_severity = 2;
  const double fatality_rate = 0.01;

  Epidemic* ep;
  Disease* variant;
  char variant_name[100];
  double transm, sever;

  FILE* output_file = fopen("landscape.txt", "w");
  
  for (transm = p_transm_min; transm <= p_transm_max; transm += p_transm_step) {

    for (sever = p_severity_min; sever <= p_severity_max; sever += p_severity_step) {
      
      printf("transm=%.3f sever=%.0f\n", transm, sever);   
      fprintf(output_file, "%f %f\n", transm, sever);
      sprintf(variant_name, "COVID19 variant transm=%.3f sever=%.0f", transm, sever);

      variant = new Disease(variant_name, transm, latency_period, contagious_duration, incubation_period, symptoms_duration, sever, fatality_rate);

      ep = new Epidemic(num_agents, num_initially_infected, base_encounters, states_fname, full_dump);

      ep->initialize();
      ep->random_infect(num_initially_infected, variant);

      ep->run_simulation(num_iterations, false);
      
      int tot_infected = num_agents - ep->states_counts[STATE_HEALTHY];
      double pct_infected = 100*(double)tot_infected/num_agents;
      printf("Population infected: %i : %.1f%%\n", tot_infected, pct_infected);


    }

  }

  fclose(output_file);


}