import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
from matplotlib import cm
import numpy as np

data = np.loadtxt("landscape.txt")

NX = 8
NY = 10

transms = data[::NY,0]
severs = data[:NY,1]
values = data[:,2].reshape((NX,NY))

xs = np.arange(len(transms))
ys = np.arange(len(severs))

# --------------------------------
# Heatmap
if True:
  
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
if True:

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
if True:

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

plt.show()