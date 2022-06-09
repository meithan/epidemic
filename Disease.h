//==============================================================================
// Agent class declaration
//==============================================================================
#ifndef DISEASE_H
#define DISEASE_H

#include <string>

class Disease {

  public:
  
  // User-defined properties of the disease
  int id;                       // A unique numeric ID for this variant
  std::string name;
  double transm_prob;           // Transmission probability (per encounter)
  double latency_period;        // Onset of contagiousness (days after infection)
  double contagious_duration;   // Duration of contagiousness (days)
  double incubation_period;     // Onset of symptoms (days after infection)
  double symptoms_duration;     // Duration of symptoms (days)
  double max_severity;          // Disease max symptoms severity (0-10)
  double fatality_rate;         // Case fatality rate
  
  // Derived properties
  double total_duration;        // Total duration of disease
  double daily_death_prob;      // The daily probability of death

  // Runtime variables
  int curr_infected;            // Agents currently infected with this disease
  int cumul_infected;           // Cumulative agents ever infected
  
  // Default constructor
  Disease(int _id, std::string p_name, double _transm_prob, double _latency_period, double _contagious_duration, double _incubation_period, double _symptoms_duration, double _max_severity, double _fatality_rate);

  // Returns the encounter transmission probabilty this many days after infection
  double get_transm_prob(double days);

  // Returns the symptoms severity this many days after initial infection
  double get_severity(double days);

  // Resets counts
  void reset_counts();
  
};
  
#endif // DISEASE
