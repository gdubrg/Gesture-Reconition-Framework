__author__ = 'Guido Borghi'

import json
from pprint import pprint
import numpy as np
import pylab as pl
import matplotlib
from matplotlib import patches
import matplotlib.pyplot as plt
import cv2

#Read file json
with open('test1.json') as data_file:
    data = json.load(data_file)

#Read ground truth file json
with open('GT.json') as data_file:
    dataGT = json.load(data_file)

#Plot settings
fig = plt.figure(figsize=(23, 8))
ax = fig.add_subplot(111)
ax.set_yticklabels([])
plt.xlim([0, data['Length'] + 1])
plt.ylim([0, 100])
plt.xlabel('Frames')
plt.title('Annotations')
plt.tight_layout()

barHeight = 8
yPositionGT = 30
yPositionAnn = 50

#Background rectangle annotations
rectMain = matplotlib.patches.Rectangle((1, yPositionAnn), data['Length']-1, barHeight, color='blue')
ax.add_patch(rectMain)

#Background rectangle GT
rectGT = matplotlib.patches.Rectangle((1, yPositionGT), dataGT['Length']-1, barHeight, color='blue')
ax.add_patch(rectGT)

#Annotations
A = data['Annotations']
for k in A:
    print k['Begin']
    print k['End']
    rectTmp = matplotlib.patches.Rectangle((k['Begin'], yPositionAnn), k['End']-k['Begin'], barHeight, color='red')
    ax.annotate(k['Type Action'], xy=(0, 0), xytext=(k['Begin']+(((k['End']-k['Begin'])/2)-1), yPositionAnn + barHeight + 1))
    ax.add_patch(rectTmp)

#GT
AGT = dataGT['Annotations']
for k in AGT:
    print k['Begin']
    print k['End']
    rectTmp = matplotlib.patches.Rectangle((k['Begin'], yPositionGT), k['End']-k['Begin'], barHeight, color='red')
    ax.annotate(k['Type Action'], xy=(0, 0), xytext=(k['Begin']+(((k['End']-k['Begin'])/2)-1), yPositionGT + barHeight + 1))
    ax.add_patch(rectTmp)

#Show chart
pl.show()

#Save the figure
fig.savefig('Annotations.png')