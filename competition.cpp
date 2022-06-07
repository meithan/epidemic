// Epidemic simulation with multi-variant competition

#include <stdio.h>

#include "Disease.h"
#include "Agent.h"
#include "Epidemic.h"

using namespace std;

int main() {

  // =====================================================================
  // GENERAL SIMULATION PARAMETERS

  const int num_agents = 10e6;
  const int num_initially_infected = 1000;    // per variant
  const int base_encounters = 10;

  bool verbose = true;

  char states_fname[] = "competition.txt";

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

  Epidemic* ep;
  Disease* variant;
  char variant_name[100];
  double transm, sever;
  FILE* file;

  // Create variants
  file = fopen("variants.txt", "w");
  std::vector<Disease*> variants;
  int next_id = 0;
  for (int i = 0; i < num_transm; i++) {   
    transm = transm_min + transm_step*i;
    for (int j = 0; j < num_sever; j++) {
      sever = severity_min + severity_step*j;  
      sprintf(variant_name, "Variant %i (t=%f, s=%f)", next_id, transm, sever);
      fprintf(file, "%i %f %f\n", next_id, transm, sever);
      variant = new Disease(next_id, variant_name, transm, latency_period, contagious_duration, incubation_period, symptoms_duration, sever, fatality_rate);
      variants.push_back(variant);
      next_id++;
    }
  }
  fclose(file);
   
  ep = new Epidemic(num_agents, variants, base_encounters, states_fname, full_dump);

  ep->initialize();

  // Create initially infected
  int stride = num_agents / num_variants;
  for (int v = 0; v < num_variants; v++) {
    for (int i = 0; i < num_initially_infected; i++) {
      ep->agents[v*stride+i]->infect(variants[v]);
      variants[v]->curr_infected += 1;
      variants[v]->cumul_infected += 1;
    }
  }

  // Run simulation
  ep->run_simulation(0, true);
  
  // Report results
  ep->report_diseases();

}