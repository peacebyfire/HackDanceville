"""
Visualize frequency data

"""
import random


def json_data_to_floor_array_shuffled(data, height, width):
    # get top 8 projects
    top_eight = sorted(data, key=lambda k: k["value"], reverse=True)[:height]

    # normalize
    total = sum([proj["value"] for proj in top_eight])

    get_freq = lambda x: int(round(width * height * x["value"] / float(total)))
    votingdata = []
    for proj in top_eight:
        freq = get_freq(proj)
        votingdata.extend([proj["color"] for i in range(freq)])
    while len(votingdata) < height * width:
        votingdata.append(top_eight[0]["color"])
    while len(votingdata) > height * width:
        votingdata.pop()
    random.shuffle(votingdata)
    return votingdata