# Uses OpenCV to make a video out of a list of images

import os
import cv2

# ==============================================================================

def make_movie(filepaths, out_fname=None, ext="mp4", fps=30, quiet=False):

  if out_fname is None:
    out_fname = f"video.{ext}"

  # Obtain images dimensions from first image
  frame = cv2.imread(filepaths[0])
  height, width, layers = frame.shape
  if not quiet:
    print(f"Image shape: {width} x {height}")

  # Initialize video encoder
  fourcc = cv2.VideoWriter_fourcc(*'mp4v')
  video = cv2.VideoWriter(out_fname, fourcc, fps, (width,height))

  # Write frames
  for fpath in filepaths:
    if not quiet:
      print(fpath)
    video.write(cv2.imread(fpath))

  video.release()
  cv2.destroyAllWindows()

  print("Wrote", out_fname)

# ==============================================================================

if __name__ == "__main__":

  import glob
  fpaths = sorted(glob.glob("images/*png"))

  make_movie(fpaths, fps=4)
