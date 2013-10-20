import time
import threading
from Queue import Empty, Queue
from hackdanceville.queue import DancefloorLoop


class Animator(DancefloorLoop):

    def __init__(self, api, data=None, delay=0.1):
        self.api = api
        if data is None:
            data = [[[0, 0, 0] for i in xrange(64)]]
        self.data = data
        self.queue = Queue()
        self.iter_data = iter(self.data)
        self.delay = delay
        super(Animator, self).__init__()

    def on_updated_data(self, data):
        if data:
            self.iter_data = iter(data)
        super(Animator, self).on_updated_data(data)

    def on_before_send(self):
        try:
            data = next(self.iter_data)
        except StopIteration:
            self.iter_data = iter(self.data)
            data = next(self.iter_data)
        return data

    def run(self):
        self.update_data()
        while self.data:
            data = self.on_before_send()
            self.api.send(data)
            time.sleep(self.delay)
            self.update_data()
