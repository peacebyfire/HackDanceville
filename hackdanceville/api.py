import time
import socket
from itertools import chain


class InvalidData(Exception):
    pass


class DancefloorAPI(object):

    def __init__(self, ip_address, port, size=64, validate=False):
        self.ip_address = ip_address
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.connected = False
        self.ENDDATA = bytearray(list('DANCEFLOOR') + [2])
        self.size = size
        self.validate = validate

    def connect(self):
        self.sock.connect((self.ip_address, self.port))
        self.connected = True

    def validate_array(self, arr):
        if not len(arr) in (self.size, self.size * 3):
            raise InvalidData('array must be length 64 or 192')
        if len(arr) == self.size:
            for el in arr:
                if len(el) != 3:
                    raise InvalidData('Element %s not length 3' % el)
                for i in el:
                    if not 255 >= i >= 0:
                        raise InvalidData('Invalid RGB value %s', str(i))

    def convert_array(self, arr):
        data = list('DANCEFLOOR') + [1] + list(chain.from_iterable(arr))
        return bytearray(data)

    def _send_raw(self, bytes):
        return self.sock.sendto(bytes, (self.ip_address, self.port))

    def send(self, arr):
        if self.validate:
            self.validate_array(arr)
        raw = self.convert_array(arr)
        self._send_raw(raw)
        self._send_raw(self.ENDDATA)


dancefloor = DancefloorAPI('192.168.1.2', 21337)


if __name__ == "__main__":
    count = 0
    while True:
        data = [[0, 0, 0] for i in xrange(64)]
        count += 1
        data[count % 64][count % 3] = 255
        dancefloor.send(data)
        time.sleep(0.1)
