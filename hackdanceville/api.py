import time
import socket
from itertools import chain


class DancefloorAPI(object):

    def __init__(self, ip_address, port, validate=False):
        self.ip_address = ip_address
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.connected = False
        self.ENDDATA = bytearray(list('DANCEFLOOR') + [2])
        self.validate = validate

    def connect(self):
        self.sock.connect((self.ip_address, self.port))
        self.connected = True

    def validate_array(self, array):
        pass

    def convert_array(self, array):
        raw = bytearray(chain('DANCEFLOOR', [1], array))
        return raw

    def _send_raw(self, bytes):
        return self.sock.sendto(bytes, (self.ip_address, self.port))

    def send(self, array):
        if self.validate:
            self.validate_array(array)
        raw = self.convert_array(array)
        self._send_raw(raw)
        self._send_raw(self.ENDDATA)


if __name__ == "__main__":
    api = DancefloorAPI('192.168.1.2', 21337)
    count = 0
    while True:
        data = [0] * 192
        count += 1
        data[count % 192] = 255
        api.send(data)
        time.sleep(0.1)
