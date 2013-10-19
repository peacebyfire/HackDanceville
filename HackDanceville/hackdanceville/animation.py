import time
import threading
from Queue import Empty, Queue


class Animator(threading.Thread):

    def __init__(self, api, data=None, delay=0.1):
        self.api = api
        if data is None:
            data = [[[0, 0, 0] for i in xrange(64)]]
        self.data = data
        self.queue = Queue()
        self.iter_data = iter(self.data)
        self.delay = delay
        super(Animator, self).__init__()

    def update_data(self):
        try:
            self.data = self.queue.get_nowait()
        except Empty:
            pass
        else:
            if self.data:
                self.iter_data = iter(self.data)

    def next(self):
        try:
            data = next(self.iter_data)
        except StopIteration:
            self.iter_data = iter(self.data)
            return self.next()
        return data

    def run(self):
        self.update_data()
        while self.data:
            data = self.next()
            self.api.send(data)
            time.sleep(self.delay)
            self.update_data()
