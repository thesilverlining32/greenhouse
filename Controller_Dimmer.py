#!/usr/bin/python3
import time
import requests
import sys

dim = 1000

#Upper Lights
R_1 = dim
G_1 = dim
B_1 = dim
#Lower Lights
R_2 = dim
G_2 = dim
B_2 = 1

#External Fan
Ex_F_1 = dim

#database check for on off time

response = requests.post('http://192.168.60.141/input', data={  'R_2': R_2,
                                                                'G_2': G_2,
                                                                'B_2': B_2,
                                                            
                                                                'R_1': R_1,
                                                                'G_1': G_1,
                                                                'B_1': B_1,
                                                                'Ex_F_1': Ex_F_1})
print (response.text)
