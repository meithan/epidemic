//==============================================================================
// Disease class implementation
//==============================================================================

#include <math.h>

#include "Disease.h"

using namespace std;

// Default constructor
Disease::Disease(int _id, string _name, double _transm_prob, double _latency_period, double _contagious_duration, double _incubation_period, double _symptoms_duration, double _max_severity, double _fatality_rate) {
  id = _id;
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
  reset_counts();
}

// Returns the encounter transmission probabilty this many days after infection
double Disease::get_transm_prob(double days) {
  if ((days < latency_period) || (days > total_duration)) {
    
    return 0.0;
  
  } else {
    
    double days_contagious = days - latency_period;

    // Constant
    // double prob = transm_prob;

    // Weight by current severity
    double sever = get_severity(days);
    double prob = transm_prob*(1+0.05*(5-sever));

    return prob;
  
  }
}

// Returns the symptoms severity this many days after initial infection
double Disease::get_severity(double days) {
  if ((days < incubation_period) || (days > total_duration)) {
    
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

void Disease::reset_counts() {

  curr_infected = 0;
  cumul_infected = 0;

}
