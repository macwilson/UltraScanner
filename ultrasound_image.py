#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Nov 14 21:43:58 2018

@author: mackenziewilson

"""

# Imports
import serial
import numpy as np
import PIL.Image as im
import time 


# Set up serial to read from Arduino
port = '/dev/cu.usbmodem1421'
ser = serial.Serial(port)

# Image size
# MAKE SURE THESE VALUES MATCH UP WITH ARDUINO CODE
horiz_steps = 6
vert_steps = 4
image_array = np.zeros( (vert_steps*2, horiz_steps) )

# Arrays to hold data (will be spatially upside down)
data_top = np.zeros((vert_steps, horiz_steps))
data_bottom = np.zeros((vert_steps, horiz_steps))

# Read from serial port until enough data captured
for i in range(vert_steps):
    for j in range(horiz_steps):
        data_bottom[i,j] = np.uint8( ser.readline().strip() )
        data_top[i,j] = np.uint8( ser.readline().strip() )

ser.close() # CLOSE THE SERIAL PORT YA DUMMY 

# Combine two transducer data arrays into one image_array
# Keep in mind the data arrays are upside down from how we want the image
for i in range(int(vert_steps/2)):
    image_array[i*4] = data_bottom[i*2]
    image_array[i*4 + 1] = data_bottom[i*2 + 1]
    image_array[i*4 + 2] = data_top[i*2]
    image_array[i*4 + 3] = data_top[i*2 + 1]

image_array_final = image_array[::-1]


# Normalize image array to pixel values
pixel_min = 0
pixel_max = 255
data_min = np.min(image_array_final)
new_data_range = max(np.max(image_array_final) - data_min, 1)
normalized_image_array = ( (image_array_final - data_min) * (255/new_data_range) ).astype(np.uint8)

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

img = im.fromarray(normalized_image_array)
img.show()

timestamp = time.strftime("%Y%m%d-%H%M%S")
img.save("saved_images/image-" + timestamp + ".bmp")


# save data for later use
np.save("saved_data/data-" + timestamp, normalized_image_array) 
# np.load can turn this back into an array for later use