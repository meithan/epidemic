import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
from matplotlib import cm
import numpy as np
import sys

data = np.loadtxt("landscape.txt")

NX = 8
NY = 10

transms = data[::NY,0]
severs = data[:NY,1]
values = data[:,2].reshape((NX,NY))

xs = np.arange(len(transms))
ys = np.arange(len(severs))

min_transm = transms.min()
max_transm = transms.max()
step_transm = transms[1] - transms[0]
min_sever = severs.min()
max_sever = severs.max()
step_sever = severs[1] - severs[0]

# --------------------------------
# Heatmap
def plot_heatmap():
  
  plt.figure(figsize=(6.5,8))

  plt.pcolormesh(xs, ys, values.T, cmap="jet")

  plt.xticks(xs, transms)
  plt.yticks(ys, severs)
  # plt.gca().invert_xaxis()
  plt.gca().set_yticklabels(ys)
  plt.xlabel("Transm. prob.")
  plt.ylabel("Severity")
  plt.gca().set_aspect("auto")

  def format_coord(x,y):
    try:
      i = int(x); j = int(y)
      t = transms[i]
      s = severs[j]
      p = values[int(x), int(y)]    
      return "transm: {:.2f}, severity: {:.0f}, infected: {:.1f}%".format(t,s,p)
    except:
      return ""
  plt.gca().format_coord = format_coord

  plt.tight_layout()

# --------------------------------
# 3D surface plot
def plot_surface():

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

# --------------------------------
# 2D curves
def plot_curves():

  plt.figure(figsize=(14,6))

  cmap = cm.jet

  plt.subplot(1,2,1)

  for j,s in enumerate(severs):
    c = cmap(j/len(severs))  
    plt.plot(transms, values[:,j], color=c, label="{:.0f}".format(s))

  plt.xlabel("Transm. prob.")
  plt.ylabel("Success (%% infected)")
  plt.title("Success vs. transmission proability")
  plt.legend(title="Severity")
  plt.grid(ls=":")

  plt.subplot(1,2,2)

  for i,t in enumerate(transms):
    c = cmap(i/len(transms))
    plt.plot(severs, values[i,:], color=c, label="{}".format(t))

  plt.xlabel("Severity")
  plt.ylabel("Success (%% infected)")
  plt.title("Success vs. disease severity")
  plt.gca().invert_xaxis()
  plt.legend(title="Transm. prob.", loc="upper left")
  plt.grid(ls=":")

  plt.tight_layout()

# --------------------------------
# 3D bar plot
def plot_3d_bar_plot():

  xs = np.arange(len(transms))
  ys = np.arange(len(severs))

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

# plot_heatmap()
# plot_surface()
# plot_curves()
plot_3d_bar_plot()

plt.show()