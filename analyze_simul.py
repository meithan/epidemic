from collections import Counter
import sys
import matplotlib.pyplot as plt
from matplotlib import colors as mcolors
import matplotlib.ticker as mticker
import numpy as np

# ------------------------------------------------------------------------------

fname = "states.txt"
full_dump = False

states = {
  0: ("Healthy", "C2"),
  1: ("Infected", "C3"),
  2: ("Deceased", "k"),
  3: ("Recovered", "C0")
}

# ------------------------------------------------------------------------------

num_states = len(states)

counts = {i: [] for i in range(num_states)}
with open(fname) as f:

  it = 0
  for line in f:

    if full_dump:  

      states = [int(x) for x in line.split()]
      counter = Counter(states)
      for i in range(num_states):
        counts[i].append(counter[i])
    
    else:
    
      values = [int(x) for x in line.split()]
      for i in range(4):
        counts[i].append(values[i])

num_agents = sum([counts[i][0] for i in range(num_states)])
for i in range(num_states):
  counts[i] = np.array(counts[i])
percents = {i: 100*counts[i]/num_agents for i in range(num_states)}

for i in range(num_states):
  name, color = states[i]
  plt.plot(percents[i], color=color, label=name)

plt.gca().yaxis.set_major_formatter(mticker.PercentFormatter())

plt.legend()
plt.grid(ls=":")
plt.tight_layout()

plt.show()