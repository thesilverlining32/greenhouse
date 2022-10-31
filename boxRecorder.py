import time
import requests
from datetime import datetime, timedelta
from threading import Timer

#define our functions
def bottomOff(R_1, G_1, B_1, Ex_F_1):
    #sets bottom to 0 and updates box   
    R_2 = G_2 = B_2 = 0

    sendIt(R_1, G_1, B_1, R_2, G_2, B_2, Ex_F_1)

def bottomOn(R_1, G_1, B_1, R_2, G_2, B_2, Ex_F_1):  
    sendIt(R_1, G_1, B_1, R_2, G_2, B_2, Ex_F_1)

def topOff(R_2, G_2, B_2, Ex_F_1):  
    #sets top to 0 and updates box
    R_1 = G_1 = B_1 = 0

    sendIt(R_1, G_1, B_1, R_2, G_2, B_2, Ex_F_1)

def topOn(R_1, G_1, B_1, R_2, G_2, B_2, Ex_F_1):   
    sendIt(R_1, G_1, B_1, R_2, G_2, B_2, Ex_F_1)

def getIt(R_1, G_1, B_1, R_2, G_2, B_2, Ex_F_1):
    #download current settings from the box
    R_1 = 0    

def sendIt(R_1, G_1, B_1, R_2, G_2, B_2, Ex_F_1):
    #download current settings from the box    
    response = requests.post('http://192.168.60.139/input', data={  'R_1':R_1,
                                                                    'G_1':G_1,
                                                                    'B_1':B_1,
                                                                    'R_2':R_1,
                                                                    'G_2':G_2,
                                                                    'B_2':B_2,
                                                                    'Ex_F_1':Ex_F_1 })
    


#initialize variables
uppR = uppG = uppB = 0
lowR = lowG = lowB = 0

intTemp = intPressure = intHumidity = 0
extTemp = extPressure = extHumidity = 0

Ex_F_1 = 0

uppOffTime = 0
uppOnTime = 0

lowOffTime = 0
lowOnTime = 0

pollTime = 1    #defines the number of minutes between polls for data and timer updates
lastPoll = 0

#main program will always execute until forcefully terminated
while 1:
    #get our current time
    now = time.time()

    #set timer
    x=datetime.today()
    y = x.replace(day=x.day, hour=1, minute=0, second=0, microsecond=0) + timedelta(days=1)
    delta_t=y-x

    secs=delta_t.total_seconds()

    t = Timer(secs, hello_world)
    t.start()

    if (now >= lastPoll):
        #update our last poll and download our new data
        lastPoll = now + pollTime * 60 * 1000
        response = requests.get('http://192.168.60.139')
        settings = response.text.split('|')
        
        for val in settings:
            values = val.split(':')
            print("values:", values[0])
            if (values[0] == "intTemp"): intTemp = values[1]
            if (values[0] == "intPressure"): intPressure = values[1]
            if (values[0] == "intHumidity"): intHumidity = values[1]

            if (values[0] == "extTemp"): extTemp = values[1]
            if (values[0] == "extPressure"): extPressure = values[1]
            if (values[0] == "extHumidity"): extHumidity = values[1]

            if (values[0] == "uppR"): uppR = values[1]
            if (values[0] == "uppG"): uppG = values[1]
            if (values[0] == "uppB"): uppB = values[1]

            if (values[0] == "lowR"): lowR = values[1]
            if (values[0] == "lowG"): lowG = values[1]
            if (values[0] == "lowB"): lowB = values[1]

            if (values[0] == "Ex_F_1"): Ex_F_1 = values[1]
        
    #print("time =", now)
    #print("minutes =", c_minute)

def hello_world():
    print 'hello world'
    #...