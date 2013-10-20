#give JSON
myJSON = {
    "project1": {
                    "value": 27, 
                    "color": [255, 0, 255]
                },
    "project2": {
                    "value": 89, 
                    "color": [0, 255, 255]
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
                    "color": [0, 0, 0]
                },
    "project6": {
                    "value": 89, 
                    "color": [0, 0, 255]
                },
    "project7": {
                    "value": 7, 
                    "color": [0, 255, 0]
                },
    "project8": {
                    "value": 2, 
                    "color": [255, 0, 0]
                },
    "project9": {
                    "value": 27, 
                    "color": [150, 0, 255]
                },
    "project10": {
                    "value": 89, 
                    "color": [150, 255, 0]
                },
    "project11": {
                    "value": 7, 
                    "color": [0, 255, 150]
                },
    "project12": {
                    "value": 2, 
                    "color": [255, 255, 255]
                }                
}
data = [{"name":proj,"value":myJSON[proj]["value"], "color":myJSON[proj]["color"]} for proj in myJSON]

def json_data_to_floor_array_shuffled(data, height, width):
    # get top 8 projects
    top_eight = sorted(data, key=lambda k: k["value"])
    
    # normalize
    total = sum([proj["value"] for proj in top_eight])
    
    val = lambda x: int(round(width*height*x["value"]/float(total)))
    votingdata = [proj["color"]*val(proj) for proj in top_eight]
    shuffle(votingdata)
    return(votingdata)