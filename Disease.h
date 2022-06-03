/*===============\\
|| Disease Class ||
\\===============*/
#ifndef DISEASE_H
#define DISEASE_H
#include <string>
#include "utils.h"
using namespace std;

class Disease {

  public:
  
  // User-defined properties of the disease
  string name;
  double transm_prob;           // Transmission probability (per encounter)
  double latency_period;        // Onset of contagiousness (days after infection)
  double contagious_duration;   // Duration of contagiousness (days)
  double incubation_period;     // Onset of symptoms (days after infection)
  double symptoms_duration;     // Duration of symptoms (days)
  double max_severity;          // Disease max symptoms severity (0-10)
  double fatality_rate;         // Case fatality rate
  
  // Derived properties
  double total_duration;
  double daily_death_prob;      // The daily probability of death
  
  // Constructor
  Disease(string p_name, double p_transm_prob, double p_latency_period, double p_contagious_duration, double p_incubation_period, double p_symptoms_duration, double p_max_severity, double p_fatality_rate) {
    name = p_name;
    transm_prob = p_transm_prob;
    latency_period = p_latency_period;
    contagious_duration = p_contagious_duration;    
    incubation_period = p_incubation_period;
    symptoms_duration = p_symptoms_duration;
    max_severity = p_max_severity;
    fatality_rate = p_fatality_rate;
    total_duration = incubation_period + symptoms_duration;
    daily_death_prob = fatality_rate / total_duration;
  }

  // Returns the symptoms severity this many days after initial infection
  double get_severity(double days) {
    if ((days < incubation_period) && (days > total_duration)) {
      return 0.0;
    } else {
      double days_symptomatic = days - incubation_period;

      // Constant at max_severity
      double severity = max_severity;

      // Linear decrease from symptoms onset to end of symptoms
      // double severity = max_severity * (1 - days_symptomatic/symptoms_duration);

      return severity;
    }
  }
  
};
  
#endif // DISEASE
