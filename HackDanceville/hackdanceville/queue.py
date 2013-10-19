import threading
from Queue import Queue, Empty
import time

from hackdanceville.api import dancefloor


class DancefloorLoop(threading.Thread):
    def __init__(self, data=None, delay=0.1):
        self.delay = delay
        self.data = data
        self.queue = Queue()
        super(DancefloorLoop, self).__init__()

    def update_data(self):
        try:
            self.data = self.queue.get_nowait()
        except Empty:
            pass

    def run(self):
        self.update_data()
        while self.data:
            dancefloor.send(self.data)
            time.sleep(self.delay)
            self.update_data()


if __name__ == "__main__":
    data = [[0, 0, 0] for i in xrange(64)]
    data[35][1] = 255
    loop = DancefloorLoop(data=data)
    loop.start()
    time.sleep(15)
    loop.queue.put(None)
    loop.join(5)