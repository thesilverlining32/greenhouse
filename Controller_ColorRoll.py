#!/usr/bin/python3
import time
import requests
import sys

t = .25 #wait time for loop

#Upper Lights
R_1 = 1023
G_1 = 1023
B_1 = 1023
#Lower Lights
R_2 = 1023
G_2 = 10
B_2 = 1023

#External Fan
Ex_F_1 = 10

state = 0

down = True
try:
    while True:
    
        if state == 0:
            #Upper Lights
            B_1 -= 50
            G_1 -= 50

            if G_2 < 1023:
                G_2 += 50
                Ex_F_1 += 50

            if B_1 < 10:
                state = 1
                R_2 = 1023
                G_2 = 1023
                B_1 = 1

                R_1 = 1023
                Ex_F_1 = 1023
                G_1 = 1
                

        if state == 1:
            R_2 -= 50
            R_1 -= 50

            if B_1 < 1023:
                B_1 += 50
                G_1 += 50
            
            if R_2 < 10:
                state = 2
                R_2 = 1
                B_1 = 1023
                G_2 = 1023

                R_1 = 1
                Ex_F_1 = 1023
                G_1 = 1023
        
        if state == 2:
            G_2 -= 50
            G_1 -= 50
            
            if R_1 < 1023:
                R_1 += 50
                R_2 += 50
            
            if G_2 < 10:
                state = 0
                R_2 = 1023
                B_1 = 1023
                G_2 = 1

                R_1 = 1023
                Ex_F_1 = 1023
                G_1 = 1

        response = requests.post('http://192.168.60.141/input', data={  'R_2': R_2,
                                                                        'G_2': G_2,
                                                                        'B_2': B_2,
                                                                    
                                                                        'R_1': R_1,
                                                                        'G_1': G_1,
                                                                        'B_1': B_1,
                                                                        'Ex_F_1': Ex_F_1})
        print (response.text)
        time.sleep(t)
except KeyboardInterrupt:
    print ('All done')
    # If you actually want the program to exit
    #raise 



