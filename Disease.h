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
  
  // Properties of the disease
  string name;
  double transm_prob;           // Transmission probability (per encounter)
  double duration;              // Duration (days)
  double contagious_onset;      // Onset of contagiousness (days)
  double symptoms_onset;        // Onset of symptoms (days)
  double severity;              // Severity (0-10)
  double fatality_rate;         // Case fatality rate
  double daily_death_prob;
  
  // Constructor
  Disease(string p_name, double p_transm_prob, double p_duration, double p_contagious_onset, double p_symptoms_onset, double p_severity, double p_fatality_rate) {
    name = p_name;
    transm_prob = p_transm_prob;
    duration = p_duration;
    contagious_onset = p_contagious_onset;
    symptoms_onset = p_symptoms_onset;
    severity = p_severity;
    fatality_rate = p_fatality_rate;
    daily_death_prob = fatality_rate / duration;
  }
  
};
  
#endif // DISEASE
