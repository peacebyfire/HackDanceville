import time
import socket


class DancefloorAPI(object):

    def __init__(self, ip_address, port):
        self.ip_address = ip_address
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.connected = False
        self.ENDDATA = bytearray(list('DANCEFLOOR') + [2])

    def connect(self):
        self.sock.connect((self.ip_address, self.port))
        self.connected = True

    def validate_array(self, array):
        pass

    def convert_array(self, array):
        data = ['D', 'A', 'N', 'C', 'E', 'F', 'L', 'O', 'O', 'R', 1] + array
        raw = bytearray(data)
        return raw

    def _send_raw(self, bytes):
        return self.sock.sendto(bytes, (self.ip_address, self.port))

    def send(self, array):
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
