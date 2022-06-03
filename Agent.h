/*=============\\
|| Agent Class ||
\\=============*/
#ifndef AGENT_H
#define AGENT_H
#include <stdio.h>
#include <math.h>
#include "Disease.h"
#include "utils.h"
using namespace std;

class Disease;

const int num_states = 4;
const int STATE_HEALTHY = 0;
const int STATE_INFECTED = 1;
const int STATE_DEAD = 2;
const int STATE_RECOVERED = 3;
const string states_names[] = {"healthy", "infected", "dead", "recovered"};

class Agent {
   
  public:
  
  // Properties of the agentt
  int ID;              // Unique sequential ID
  int state;           // Current infectious state
  bool isAlive;        // Is the agent still alive?
  bool isInfected;     // Is the agent infected?
  bool isContagious;   // Is the agent contagious?
  bool isSymptomatic;  // Is the agent symptomatic?
  bool isImmune;       // Is the agent immune?
  Disease* disease;    // The disease this agent has, if any
  double days_infected;   // Infection day the agent is on
  int base_encounters;   // Number of encounters per day when healthy
  
  // Constructor
  Agent (int p_ID, int p_base_encounters) {
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
  void repr(char* retbuf) {
    sprintf(retbuf, "<#%i: state=%s>", ID, states_names[state].c_str());
  }

  // Infects the agent with the given disease
  void infect(Disease* _disease) {
    if (isInfected) {
      // Already infected!
      return;
    }
    isInfected = true;
    state = STATE_INFECTED;
    days_infected = 0;
    disease = _disease;
    if (disease->latency_period == 0) isContagious = true;
    if (disease->incubation_period == 0) isSymptomatic = true;
  }

  // Determines how many encounters this agent will have, based on
  // the disease's severity (if any)
  int get_encounters() {
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
  void evolve_disease() {
    
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
  
};
  
#endif // AGENT_H
