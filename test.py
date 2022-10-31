#!/usr/bin/python3
import time
import requests
import sys

t = .5 #wait time for loop

#Upper Lights
value = 50

post = False

try:
    if post:
        while True:
            value += 10

            response = requests.post('http://thesilverlining.ddnsking.com:8000/piPost.php', data={ 'value': value })
            print (response.text)
            time.sleep(t)
        
    else:
        response = requests.get('http://thesilverlining.ddnsking.com:8000/piGet.php')
        print (response.text)
            
except KeyboardInterrupt:
    print ('All done')
    # If you actually want the program to exit
    #raise 




