import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
import matplotlib.colors as mcolors
from matplotlib import cm
import numpy as np
from math import ceil
import sys

NX = 8
NY = 10
params = np.loadtxt("variants.txt")[:,1:]
transms = params[::NY,0]
severs = params[:NY,1]
min_transm = transms.min()
max_transm = transms.max()
step_transm = transms[1] - transms[0]
min_sever = severs.min()
max_sever = severs.max()
step_sever = severs[1] - severs[0]
num_variants = NX*NY
print(transms)
print(severs)

data = np.loadtxt("competition.txt")
pop = data[0,0] + data[0,1]
healthy = data[:,0]
infected = data[:,1]
recovered = data[:,2]
dead = data[:,3]
counts = data[:,4:4+num_variants]
cumuls = data[:,4+num_variants:]

# --------------------------------
# Time evolution of global counts
def plot_time_basic():

  plt.figure(figsize=(10,6))

  plt.plot(healthy/pop*100, label="healthy", color="green")
  plt.plot(infected/pop*100, label="infected", color="red")
  plt.plot(recovered/pop*100, label="recovered", color="blue")
  plt.plot(dead/pop*100, label="dead", color="black")

  plt.xlabel("Days")
  plt.ylabel("% population")
  plt.grid(ls=":")
  plt.gca().yaxis.set_major_formatter(mticker.PercentFormatter())
  plt.legend()
  plt.tight_layout()

  if "--save" in sys.argv:
    fname = "../competition_time_basic.png"
    plt.savefig(fname)
    print("Wrote", fname)

# --------------------------------
# Time evolution of counts
def plot_time_counts():

  plt.figure(figsize=(14,10))

  for i in range(num_variants):
    transm, sever = params[i]
    R = (transm-min_transm)/(max_transm-min_transm)
    G = 0
    B = 1 - (sever-min_sever)/(max_sever-min_sever)
    plt.plot(counts[:,i]/pop*100, color=(R, G, B))

  plt.xlabel("Days")
  plt.ylabel("% population")
  plt.grid(ls=":")
  plt.gca().yaxis.set_major_formatter(mticker.PercentFormatter())
  plt.tight_layout()

  if "--save" in sys.argv:
    fname = "../competition_time_counts.png"
    plt.savefig(fname)
    print("Wrote", fname)

# --------------------------------
# Time evolution of cumulative counts
def plot_time_cumul():

  plt.figure(figsize=(14,10))

  for i in range(num_variants):
    transm, sever = params[i]
    R = (transm-min_transm)/(max_transm-min_transm)
    G = 0
    B = 1 - (sever-min_sever)/(max_sever-min_sever)
    plt.plot(cumuls[:,i]/pop*100, color=(R, G, B))

  plt.xlabel("Days")
  plt.ylabel("% population, cumulative")
  plt.grid(ls=":")
  plt.gca().yaxis.set_major_formatter(mticker.PercentFormatter())
  plt.tight_layout()

  if "--save" in sys.argv:
    fname = "../competition_time_cumul.png"
    plt.savefig(fname)
    print("Wrote", fname)

# --------------------------------
# Surface plot
def plot_surface():

  xs = np.arange(len(transms))
  ys = np.arange(len(severs))
  values = cumuls[-1].reshape(NX, NY)/pop*100

  plt.figure(figsize=(10,10))
  ax = plt.axes(projection='3d')

  plt.xticks(xs, transms)
  plt.yticks(ys, severs)

  X, Y = np.meshgrid(xs, ys)
  Z = values.T
  if True:
    surf = ax.plot_surface(X, Y, Z, cmap="jet", linewidth=0, antialiased=False)
  else:
    ax.contourf(X, Y, Z, zdir='z', offset=0, cmap="jet")
    ax.contourf(X, Y, Z, zdir='x', offset=-1, cmap="jet")
    ax.contourf(X, Y, Z, zdir='y', offset=0, cmap="jet")

  ax.set_xlabel("Transm. prob.")
  ax.set_ylabel("Severity")
  ax.set_zlabel("Success (%% infected)")
  ax.view_init(elev=35, azim=135)

  if "--save" in sys.argv:
    fname = "../competition_surface.png"
    plt.savefig(fname)
    print("Wrote", fname)  

# --------------------------------
# 3D bar plot
def plot_3d_bar_plot():

  xs = np.arange(len(transms))
  ys = np.arange(len(severs))
  values = cumuls[-1].reshape(NX, NY)/pop*100

  X = []; Y = []; Z = []
  colors = []
  for i in range(len(transms)):
    for j in range(len(severs)):
      X.append(transms[i])
      Y.append(severs[j])
      Z.append(values[i,j])
      R = 1 - (severs[j]-min_sever)/(max_sever-min_sever)
      G = 0
      B = (transms[i]-min_transm)/(max_transm-min_transm)
      colors.append((R,G,B))
  X = np.array(X); Y = np.array(Y); Z = np.array(Z)
  zero = np.zeros_like(Z)

  plt.figure(figsize=(10,10))
  ax = plt.axes(projection='3d')

  # plt.xticks(xs, transms)
  # plt.yticks(ys, severs)

  dx = step_transm/2
  dy = step_sever/2
  ax.bar3d(X-dx/2, Y-dy/2, zero, dx, dy, Z, colors, shade=False)

  ax.set_xlabel("Transm. prob.")
  ax.set_ylabel("Severity")
  ax.set_zlabel("Success (%% infected)")
  ax.view_init(elev=35, azim=135)

  if "--save" in sys.argv:
    fname = "../competition_3dbars.png"
    plt.savefig(fname)
    print("Wrote", fname)

# --------------------------------
# Heatmap
def plot_heatmap():

  xs = np.arange(len(transms))
  ys = np.arange(len(severs))
  values = (cumuls[-1].reshape(NX, NY))/pop*100
  print(values)

  plt.figure(figsize=(8,8))

  plt.pcolormesh(xs, ys, values.T, cmap="jet", norm=mcolors.LogNorm())

  plt.colorbar()

  plt.xticks(xs, transms)
  plt.yticks(ys, severs)
  # plt.gca().invert_yaxis()
  # plt.gca().set_yticklabels(ys)
  plt.xlabel("Transmissibility")
  plt.ylabel("Severity")
  plt.title("Multi-variant competition | Success vs. transm. & severity")
  # plt.gca().set_aspect("auto")

  def format_coord(x,y):
    try:
      i = int(x+0.5); j = int(y+0.5)
      t = transms[i]
      s = severs[j]
      p = values[i,j]
      return "transm: {:.2f}, severity: {:.0f}, infected: {:.3f}%".format(t,s,p)
    except:
      return ""
  plt.gca().format_coord = format_coord

  plt.tight_layout()

  if "--save" in sys.argv:
    fname = "../competition_heatmap.png"
    plt.savefig(fname)
    print("Wrote", fname)

# --------------------------------

# plot_time_basic()
# plot_time_counts()
# plot_time_cumul()
# plot_surface()
plot_3d_bar_plot()
# plot_heatmap()

plt.show()