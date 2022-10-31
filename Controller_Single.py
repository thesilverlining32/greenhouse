#!/usr/bin/python3
import time
import requests
import sys

#Upper Lights
R_1 = 750
G_1 = 750
B_1 = 750
#Lower Lights
R_2 = 750
G_2 = 750
B_2 = 750

#External Fan
Ex_F_1 = 750

#database check for on off time

response = requests.post('http://192.168.60.141/input', data={  'R_2': R_2,
                                                                'G_2': G_2,
                                                                'B_2': B_2,
                                                            
                                                                'R_1': R_1,
                                                                'G_1': G_1,
                                                                'B_1': B_1,
                                                                'Ex_F_1': Ex_F_1})
print (response.text)
