import json
from hackdanceville.api import dancefloor

class Animation:
    def updateFrame(json_frame):
        frame_data = []
        #try:
            frame_data = json.loads(json_frame)
        #except ValueError, e:
        #    print 'JSON ERROR!'
        #    return
        #else:
        #    print 'JSON ERROR!'
        #    return
        data = frame_data['data']
        dancefloor.send(data)
        