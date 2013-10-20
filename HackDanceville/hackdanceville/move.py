from hackdanceville.queue import DancefloorLoop

VICTORY_SCREENS = {
    "player1": [[0,0,0],[0,0,0],[0,255,0],[0,255,0],[0,255,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,255,0],[0,255,0],[0,0,0],[0,255,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,255,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,255,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,255,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,255,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,255,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,255,0],[0,255,0],[0,255,0],[0,255,0],[0,255,0],[0,255,0],[0,0,0]],
    "player2": [[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,255],[0,0,255],[0,0,255],[0,0,255],[0,0,0]]
}


class BaseBomberman(DancefloorLoop):

    def __init__(self, api=None, delay=0.1):
        super(BaseBomberman, self).__init__(api, True, delay)
        self.players = {}
        self.reset()

    def reset(self):
        self.data = True
        self.gameover = False
        self.victory_count = 0

    def kill_player(self, player_name):
        del self.players[player_name]
        if len(self.players) == 1:
            self.data = VICTORY_SCREENS[self.players.keys()[0]]
            self.gameover = True

    def check_explosion(self, bomb):
        for pname, player in self.players.items():
            if abs(player.x - bomb.x) <= 2 and abs(player.y - bomb.y) <= 2:
                self.kill_player(pname)

    def compile_data(self):
        data = [[0, 0, 0] for i in xrange(64)]
        for player in self.players.values():
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
        if not self.gameover:
            data = self.compile_data()
        else:
            self.victory_count += 1
            data = self.data
            if self.victory_count > 40:
                self.reset()
        return data


class SingleKeyboardBomberman(BaseBomberman):

    def reset(self):
        super(SingleKeyboardBomberman, self).reset()
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
        self.bombSet = True
        self.exploded = False

    def return_data(self):
        if self.bombSet == True:
            self.blinkCount += 1
            if self.blinkCount > 20:
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

    def find_pos(self, curPos, method):
        if method == '-':
            newPos = curPos - 1
        else:
            newPos = curPos + 1
        if newPos < 8 and newPos > -1:
            return newPos
        else:
            return curPos

    def move_left(self):
        self.x = self.find_pos(self.x, '-')

    def move_right(self):
        self.x = self.find_pos(self.x, '+')

    def move_up(self):
        self.y = self.find_pos(self.y, '-')

    def move_down(self):
        print 'move down'
        self.y = self.find_pos(self.y, '+')

    def setBomb(self):
        if len(self.bombs) < 4:
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
            thisBomb = el.return_data()
            returnObj['bombs'].append(thisBomb)
        return returnObj
