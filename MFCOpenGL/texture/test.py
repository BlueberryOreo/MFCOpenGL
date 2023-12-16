from PIL import Image
import numpy as np

img = Image.open("./8081_earthmap2k.jpg")
img_arr = np.array(img)
print(img_arr.shape)