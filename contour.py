from __future__ import print_function
import scipy
from scipy import ndimage
from skimage import feature

import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('snowflake.png',0)
height, width = img.shape[:2]
#img = cv2.resize(img,(0,0),fx=0.5,fy=0.5,interpolation = cv2.INTER_CUBIC)
img = scipy.misc.imresize(img,(200,200))

contour = plt.contour(img)
path = contour.collections[2].get_paths()[0]

v = path.vertices
x = v[:,0]; np.append(x,v[0][0])
y = v[:,1]; np.append(y,v[0][1])

f = open('f','w')

for i in range(0,len(x)):
    print(str(int(x[i])), file=f)
    print(str(int(y[i])), file=f)

#plt.imshow(img)
#plt.scatter(x,y)
#plt.show()






