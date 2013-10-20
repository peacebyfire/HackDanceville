def json_data_to_floor_array(data, height, width):
    # get top 8 projects
    top_eight = sorted(data, key=lambda k: k["value"])[-8:]
    
    # normalize
    total = top_eight[-1]["value"]
    
    val = lambda x: int(round(height*x["value"]/float(total)))
    return [proj["color"]*val(proj) + (height-val(proj))*[0,0,0] for proj in top_eight]