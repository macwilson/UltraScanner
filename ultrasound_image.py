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
import random



def normalize_array(arr):
    '''
        Old way of turning distance data into pixel values. 
        
        This function takes the range of the data captured and adjusts the
        data so that it has a new range of 255, i.e. 8-bit pixel values.
        Larger distances correspond to lower pixell values (less intensity
        indicates further away).
        Returns array with datatype of unsigned integer, 8-bit (max 265).
    '''
    pixel_max = 255
    data_min = np.min(arr)
    new_data_range = max(np.max(arr) - data_min, 1)
    new_arr = (arr - data_min) * (pixel_max/new_data_range)
    return (pixel_max-new_arr).astype(np.uint8)


def bin_pixels(arr):
    '''
        This function is the preferred method of turning distance data
        into pixel values. 
        
        This function uses the physical usable imaging range (which for ease 
        of use is set to 255mm) and maps all distances to 0-255 within that 
        range. Each pixel value corresponds to a distance change of 1mm.
        Everything below the lower limit will be set to 255 intensity, and 
        everything above the upper limit will be set to 0 intensity. 
        Returns array with datatype unsigned integer, 8-bit
    '''
    assert(type(arr) == np.ndarray)
    arr[arr < 150] = 500 # THRESHOLDING FOR THE BACKGROUND THAT IS MISSED
    min_dist = 130 # in mm
    #max_dist = 385
    pixel_min = 0
    pixel_max = 255
    
    result_arr = arr.astype(int) - min_dist
    result_arr[result_arr < pixel_min] = pixel_min
    result_arr[result_arr > pixel_max] = pixel_max
    return (255-result_arr).astype(np.uint8)


def build_image_array(bottom, top, h, v):
    '''
        This function takes data that has been separated into data from the
        top and bottom transducers and combines them into one big 2D image 
        array. This requires inherent knowledge of the motion pattern of the
        system.
        Returns 2D array with data type int, as designated in bottom and top.
    '''
    # Combine two transducer data arrays into one image_array
    # Keep in mind the data arrays are upside down from how we want the image
    im_arr = np.zeros( (v*2, h) )
    for i in range(int(v/2)):
        im_arr[i*4] = bottom[i*2]
        x = bottom[i*2 + 1]
        im_arr[i*4 + 1] = x[::-1] # This row went backwards
        im_arr[i*4 + 2] = top[i*2]
        y = top[i*2 + 1]
        im_arr[i*4 + 3] = y[::-1] # This row went backwards
        
    return im_arr[::-1]


def show_image(arr):
    # This also saves the data and image
    img = im.fromarray(arr)
    img.show()
    
    
def save_image(arr):
    img = im.fromarray(arr)
    timestamp = time.strftime("%Y%m%d-%H%M%S")
    img.save("saved_images/image-" + timestamp + ".bmp")
    np.save("saved_data/data-" + timestamp, arr) 
    
def load_image_data(path, horiz_steps=10):
    # Normalized numpy files
    arr = (np.load(path)).astype(np.uint8)
    return arr.reshape(int(arr.size / horiz_steps), horiz_steps )


def load_raw_data(path, h=10, v=8, col=1):
    arr = np.genfromtxt(path, delimiter=",", usecols=col, skip_header=1)
    bot = arr[0::2]
    bot = bot[~np.isnan(bot)]
    top = arr[1::2]   
    top = top[~np.isnan(top)]
    
    return build_image_array(bot.reshape(v,h), top.reshape(v,h), h, v)


def load_raw_data_horizontal_testing(path, col=1):
    arr = np.genfromtxt (path, delimiter=",", usecols=col, skip_header=1)
    bot = arr[0::2]
    bot = bot[~np.isnan(bot)]
    top = arr[1::2]   
    top = top[~np.isnan(top)]
    
    im_arr = np.zeros((2, len(bot)))
    im_arr[0] = top
    im_arr[1] = bot
    
    return im_arr

        
if __name__ == '__main__':
    # Set up serial to read from Arduino
    port = '/dev/cu.usbmodem1421'
    ser = serial.Serial(port)
    
    # Image size
    # MAKE SURE THESE VALUES MATCH UP WITH ARDUINO CODE
    horiz_steps = 15
    vert_steps = 12
    
    # Arrays to hold data (will be spatially upside down)
    data_top = np.zeros((vert_steps, horiz_steps))
    data_bottom = np.zeros((vert_steps, horiz_steps))


    while True:
        
        # Do not start until Arduino tells us to
        if np.uint8( ser.readline().strip() ) == np.uint8(99999):
            
            # START.
            # Read from serial port until enough data captured
            for i in range(vert_steps):
                for j in range(horiz_steps):
                    data_bottom[i,j] = np.uint8( ser.readline().strip() )
                    print(data_bottom[i,j])
                    data_top[i,j] = np.uint8( ser.readline().strip() )
                    print(data_top[i,j])
            
            # Combine data into image array
            image_array = build_image_array(data_bottom, data_top, horiz_steps, vert_steps)
            np.save("saved_data/raw-data2", image_array) 
            
            # Normalize image array to pixel values
            bin_image_array = bin_pixels(image_array)
    
            # Make image
            show_image(bin_image_array)
            save_image(bin_image_array)
            
