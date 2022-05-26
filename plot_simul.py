import sys
import matplotlib.pyplot as plt
from matplotlib import colors as mcolors
import numpy as np
from PIL import Image

fname = "states.txt"

NX = 1000
NY = 1000
square_size = 1

width = NX*square_size
height = NY*square_size

def to_rgb_255(color):
  r,g,b = mcolors.to_rgb(color)
  return (int(r*255), int(g*255), int(b*255))

colors = {
  0: to_rgb_255('gray'),
  1: to_rgb_255('red'),
  2: to_rgb_255('black'),
  3: to_rgb_255('lime')
}

with open(fname) as f:
  
  it = 0
  for line in f:
  
    states = np.array([int(x) for x in line.split()], dtype=int).reshape((NX,NY))
    image_data = np.zeros((height, width, 3), dtype=np.uint8)
    for i in range(height):
      for j in range(width):
        x = int(i // square_size)
        y = int(j // square_size)
        color = colors[states[x,y]]
        # print(x, y, states[x,y], color)
        image_data[i, j] = color

    img = Image.fromarray(image_data, "RGB")
    
    if "--save" in sys.argv:
      out_fname = "images/states_{:03d}.png".format(it)
      img.save(out_fname)
    else:
      img.show()

    print("it=", it)
    it += 1