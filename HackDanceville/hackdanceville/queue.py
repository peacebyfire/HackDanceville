import threading
from Queue import Queue, Empty
import time

from hackdanceville.api import dancefloor


class DancefloorLoop(threading.Thread):
    def __init__(self, api=None, data=None, delay=0.1):
        if api is None:
            api = dancefloor
        self.api = api
        self.delay = delay
        self.data = data
        self.queue = Queue()
        self.dead = False
        super(DancefloorLoop, self).__init__()

    # some hooks
    def on_updated_data(self, data):
        pass

    def on_before_send(self):
        return self.data

    def update_data(self):
        try:
            self.data = self.queue.get_nowait()
        except Empty:
            pass
        else:
            self.on_updated_data(self.data)

    def run(self):
        self.update_data()
        while self.data:
            data = self.on_before_send()
            self.api.send(data)
            time.sleep(self.delay)
            self.update_data()

    def kill(self):
        self.queue.put(None)
        self.join(5)
        self.dead = True


if __name__ == "__main__":
    data = [[0, 0, 0] for i in xrange(64)]
    data[35][1] = 255
    loop = DancefloorLoop(data=data)
    loop.start()
    time.sleep(15)
    loop.kill()
