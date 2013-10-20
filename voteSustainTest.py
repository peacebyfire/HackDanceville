#give JSON
myJSON = {
    "project1": {
                    "value": 27, 
                    "color": [255, 0, 255]
                },
    "project2": {
                    "value": 89, 
                    "color": [0, 0, 255]
                },
    "project3": {
                    "value": 7, 
                    "color": [255, 255, 0]
                },
    "project4": {
                    "value": 2, 
                    "color": [255, 255, 255]
                },
    "project5": {
                    "value": 27, 
                    "color": [255, 0, 255]
                },
    "project6": {
                    "value": 89, 
                    "color": [0, 0, 255]
                },
    "project7": {
                    "value": 7, 
                    "color": [255, 255, 0]
                },
    "project8": {
                    "value": 2, 
                    "color": [255, 255, 255]
                },
    "project9": {
                    "value": 27, 
                    "color": [255, 0, 255]
                },
    "project10": {
                    "value": 89, 
                    "color": [0, 0, 255]
                },
    "project11": {
                    "value": 7, 
                    "color": [255, 255, 0]
                },
    "project12": {
                    "value": 2, 
                    "color": [255, 255, 255]
                }                
}
data = [{"name":proj,"value":myJSON[proj]["value"], "color":myJSON[proj]["color"]} for proj in myJSON]

#from hackdanceville.api import dancefloor
from hackdanceville.queue import DancefloorLoop
from  votingHistogram import json_data_to_floor_array
from itertools import chain
data2 = json_data_to_floor_array(data, 8, 8)
vote_loop = DancefloorLoop(data=data2)
vote_loop.start()
vote_loop.join()