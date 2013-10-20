import curses

from hackdanceville.queue import DancefloorLoop


def movement_wrapper(func):
    def wrapper(inst, *args, **kwargs):
        print inst.player1Data
        """inst.data[inst.y * 8 + inst.x][inst.color_i] = 0
        func(inst, *args, **kwargs)
        inst.data[inst.y * 8 + inst.x][inst.color_i] = 255"""
        # check to see if player is on top of bomb
        pass
    return wrapper


class BaseBomberman(DancefloorLoop):

    def __init__(self, api=None, delay=0.1):
        data = True
        super(BaseBomberman, self).__init__(api, data, delay)
        self.players = {}
        self.bombs = []

    def check_explosion(self, bomb):
        pass

    def compile_data(self):
        data = [[0, 0, 0] for i in xrange(64)]
        for pname, player in self.players.items():
            player_data = player.returnData()
            cell = player_data["y"] * 8 + player_data["x"]
            data[cell] = player_data["color"]
            for bomb in player_data["bombs"]:
                cell = bomb.y * 8 + bomb.x
                data[cell] = bomb.color
                if bomb.exploded:
                    self.check_explosion(bomb)
        return data

    def on_before_send(self):
        return self.compile_data()


class SingleKeyboardBomberman(BaseBomberman):

    def __init__(self, api=None, delay=0.1):
        super(SingleKeyboardBomberman, self).__init__(api, delay)
        self.players['player1'] = Player([0, 255, 0], [7,0])
        self.players['player2'] = Player([0, 0, 255], [0,7])
        self.keymap = {
            16: self.players['player1'].setBomb,
            38: self.players['player1'].move_up,
            40: self.players['player1'].move_down,
            37: self.players['player1'].move_left,
            39: self.players['player1'].move_right,
            71: self.players['player2'].setBomb,
            83: self.players['player2'].move_up,
            70: self.players['player2'].move_down,
            65: self.players['player2'].move_left,
            68: self.players['player2'].move_right,
            113: self.kill
        }

    def put(self, key):
        if key in self.keymap:
            func = self.keymap[key]
            func()


class Bomb(object):

    def __init__(self, player):
        self.player = player
        self.x = player.x
        self.y = player.y
        self.color = [255, 0, 0]
        self.blinkCount = 0
        self.bombSet = False
        self.exploded = False

    def set_bomb(self, playerPosX, playerPosY):
        if self.bombSet == False:
            print 'bomb set'
            self.x = playerPosX
            self.y = playerPosY
            self.bombSet = True
            self.exploded = False

    def return_data(self):
        if self.bombSet == True:
            self.blinkCount += 1
            if self.blinkCount > 5:
                self.exploded = True
                self.bombSet = False
                self.blinkCount = 0
                self.player.removeBomb()
        else:
            self.exploded = False
        return self


class Player(object):

    def __init__(self, color, initialPos):
        self.x = initialPos[0]
        self.y = initialPos[1]
        self.color = color
        self.bombs = []

    def move_left(self):
        self.x = (self.x - 1) % 8

    def move_right(self):
        self.x = (self.x + 1) % 8

    def move_up(self):
        self.y = (self.y - 1) % 8

    def move_down(self):
        self.y = (self.y + 1) % 8

    def setBomb(self):
        if self.bombs.count < 4:
            self.bombs.append(Bomb(self))

    def removeBomb(self):
        del self.bombs[0]

    def returnData(self):
        returnObj = {}
        returnObj['x'] = self.x
        returnObj['y'] = self.y
        returnObj['color'] = self.color
        returnObj['bombCount'] = len(self.bombs)
        returnObj['bombs'] = []
        for el in self.bombs:
            thisBomb = el.returnData()
            returnObj['bombs'].append(thisBomb)
        return returnObj
