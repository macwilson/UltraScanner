#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Nov 14 21:43:58 2018

@author: mackenziewilson
"""

import serial
import matplotlib.pyplot as plt
import matplotlib.colors as colour
import numpy as np
import PIL.Image as im


# Set up serial to read from Arduino
port = '/dev/cu.usbmodem1421'
ser = serial.Serial(port)

# Image size
cols = 5
rows = 5

# Read from serial port until enough data captured
dataTop = []
dataBottom = []
for i in range(rows*cols):
    valBottom, valTop = (np.uint8( ser.readline().strip() )).split(',')
    dataBottom.append(valBottom)
    dataTop.append(valTop)

ser.close() # CLOSE THE SERIAL PORT YA DUMMY 

# Convert data to 2D array
data_array = np.array(data)
data_array = data_array[::-1].reshape((rows,cols))
#print(data_array)

# Normalize data to pixel values
pixel_min = 0
pixel_max = 255
data_min = np.min(data_array)
new_data_range = max(np.max(data_array) - data_min, 1)
normalized_data_array = ( (data_array - data_min) * (255/new_data_range) ).astype(np.uint8)

'''
# For ease of use
normalized_data_array = np.array(
[[139, 126, 101,  98,  85],
 [ 65,  54,  57,  87,  27],
 [  8,   0,  49, 180, 252],
 [254, 254, 254, 254, 254],
 [254, 254, 254, 254, 254]], dtype=np.uint8)
'''
#print("")
#print(normalized_data_array)

img = im.fromarray(normalized_data_array)
img.show()
img.save('image_1.bmp')


