//==============================================================================
// Agent class implementation
//==============================================================================

#include <stdio.h>
#include <math.h>

#include "Agent.h"

using namespace std;

// Defualt constructor
Agent::Agent (int p_ID, int p_base_encounters) {
  ID = p_ID;
  state = STATE_HEALTHY;
  isAlive = true;
  isInfected = false;
  isContagious = false;
  isSymptomatic = false;    
  isImmune = false;
  base_encounters = p_base_encounters;
}

// Returns a string representation of the agent in the provided char*
void Agent::repr(char* retbuf) {
  sprintf(retbuf, "<#%i: state=%s>", ID, states_names[state].c_str());
}

// Infects the agent with the given disease
void Agent::infect(Disease* _disease) {
  if ((!isAlive) || isInfected || isImmune) return;
  isInfected = true;
  state = STATE_INFECTED;
  days_infected = 0;
  disease = _disease;
  if (disease->latency_period == 0) isContagious = true;
  if (disease->incubation_period == 0) isSymptomatic = true;
}

// Determines how many encounters this agent will have, based on
// the disease's severity (if any)
int Agent::get_encounters() {
  if ((!isInfected) || (!isSymptomatic)) {
    return base_encounters;
  } else {
    // A function going from base_encounters at severity=0
    // to 0 at severity=10
    // Simple linear function 
    // return base_encounters * (1 - disease->severity/10.0);
    // Quadratic function
    double severity = disease->get_severity(days_infected);
    return int(round(base_encounters * (1 - pow(severity/10.0, 2))));
  }
}

// Evolves the disease of the agent (if any)
void Agent::evolve_disease() {
  
  if (!isInfected) return;

  // See if agent dies
  if (randreal() <= disease->daily_death_prob) {
    isInfected = false;
    isContagious = false;
    isSymptomatic = false;
    isImmune = true;
    isAlive = false;
    state = STATE_DEAD;
    return;
  }

  days_infected++;

  if (days_infected > disease->total_duration) {
    
    isInfected = false;
    isContagious = false;
    isSymptomatic = false;
    isImmune = true;
    state = STATE_RECOVERED;

  } else {

    if ((!isContagious) && (days_infected >= disease->latency_period)) {
      isContagious = true;
    }

    if ((!isSymptomatic) && (days_infected >= disease->incubation_period)) {
      isSymptomatic = true;
    }

  }

}