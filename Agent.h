//==============================================================================
// Agent class declaration
//==============================================================================
#ifndef AGENT_H
#define AGENT_H

#include "utils.h"
#include "Disease.h"

const int num_states = 4;
const int STATE_HEALTHY = 0;
const int STATE_INFECTED = 1;
const int STATE_DEAD = 2;
const int STATE_RECOVERED = 3;
const std::string states_names[] = {"healthy", "infected", "dead", "recovered"};

class Agent {
   
  public:

  // -----------------------------------------------------------------------
  // CLASS VARIABLES

  // Properties of the agentt
  int ID;                    // Unique sequential ID
  int state;                 // Current infectious state
  bool isAlive;              // Is the agent still alive?
  bool isInfected;           // Is the agent infected?
  bool isContagious;         // Is the agent contagious?
  bool isSymptomatic;        // Is the agent symptomatic?
  bool isImmune;             // Is the agent immune?
  Disease* disease;          // The disease this agent has, if any
  double days_infected;      // Infection day the agent is on
  int base_encounters;       // Number of encounters per day when healthy  
  
  // -----------------------------------------------------------------------
  // CLASS METHODS  
    
  // Defualt constructor    
  Agent (int _ID, int _base_encounters);
  
  // A string representation of the Agent
  void repr(char* retbuf);
  
  // Infects the agent with the disease
  void infect(Disease* _disease);
  
  // Compute number of encounters (based on disease state)
  int get_encounters();
  
  // Evolves the Agent's disease (if any)
  void evolve_disease();
  
};
  
#endif // AGENT_H