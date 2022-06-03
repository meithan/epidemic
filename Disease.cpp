//==============================================================================
// Disease class implementation
//==============================================================================

#include <math.h>

#include "Disease.h"

using namespace std;

// Default constructor
Disease::Disease(string _name, double _transm_prob, double _latency_period, double _contagious_duration, double _incubation_period, double _symptoms_duration, double _max_severity, double _fatality_rate) {
  name = _name;
  transm_prob = _transm_prob;
  latency_period = _latency_period;
  contagious_duration = _contagious_duration;    
  incubation_period = _incubation_period;
  symptoms_duration = _symptoms_duration;
  max_severity = _max_severity;
  fatality_rate = _fatality_rate;
  total_duration = incubation_period + symptoms_duration;
  daily_death_prob = fatality_rate / total_duration;
}

  // Returns the symptoms severity this many days after initial infection
double Disease::get_severity(double days) {
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
