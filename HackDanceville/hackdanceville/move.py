import curses

from hackdanceville.queue import DancefloorLoop

def movement_wrapper(func):
    def wrapper(inst, *args, **kwargs):
        inst.data[inst.y * 8 + inst.x][inst.color_i] = 0
        func(inst, *args, **kwargs)
        inst.data[inst.y * 8 + inst.x][inst.color_i] = 255
        # check to see if player is on top of bomb
    return wrapper


class Move(object):

    def __init__(self):
        self.init_data()
        self.init_bomb()
        self.keymap = {
            curses.KEY_UP: self.player1_move_up,
            curses.KEY_DOWN: self.player1_move_down,
            curses.KEY_LEFT: self.player1_move_left,
            curses.KEY_RIGHT: self.player1_move_right,
            99: self.change_color,
            113: self.quit
        }
        self.color_i = 1
        self.loop = None
        self.go = False

    def init_data(self):
        self.player1Data = [[0, 255, 0]]
        for i in xrange(63):
            self.player1Data.append([0, 0, 0])
        self.player2Data = []
        for i in xrange(63):
            self.player2Data.append([0, 0, 0])
        self.player2Data.append([0, 255, 0])
        self.player1X = 0
        self.player1Y 0
        self.player2X = 0
        self.player2Y 0
        self.player1Bomb = Bomb();
        self.player2Bomb = Bomb();

    @movement_wrapper
    def player1_move_left(self):
        self.player1X = (self.player1X - 1) % 8

    @movement_wrapper
    def player1_move_right(self):
        self.player1X = (self.player1X + 1) % 8

    @movement_wrapper
    def player1_move_up(self):
        self.player1Y = (self.player1Y - 1) % 8

    @movement_wrapper
    def player1_move_down(self):
        self.player1Y = (self.player1Y + 1) % 8

    @movement_wrapper
    def change_color(self):
        self.color_i = 2 if self.color_i == 1 else 1

    def initialize_loop(self):
        if not self.loop or not self.loop.is_alive():
            self.loop = DancefloorLoop(data=self.player1Data)
            self.loop.start()
            self.go = True

    def quit(self):
        self.player1Data = None
        self.player2Data = None
        self.go = False

    def put(self, key):
        print 'putting ' + str(key)
        if key in self.keymap:
            func = self.keymap[key]
            func()
            print 'ran ' + str(func)
        self.loop.queue.put(self.player1Data)

    def curses_loop(self):
        stdscr = curses.initscr()
        curses.cbreak()
        stdscr.keypad(1)

        stdscr.addstr(0, 10, "Hit 'q' to quit")
        stdscr.refresh()

        self.initialize_loop()

        while self.go:
            key = stdscr.getch()
            stdscr.addch(20, 25, key)
            stdscr.refresh()
            self.put(key)

        curses.endwin()

class Bomb(object):

    self.x = 0
    self.y = 0
    self.exploded = False

    def __init__(self):
        self.init_bomb()

    def init_bomb(self):
        self.bombData = []
        for i in xrange(64):
            self.bombData.append([0, 0, 0])
        self.blasted = []

    def collision(self, playerPosX, playerPosY):
        if self.explosded == True AND self.x == playerPosX AND self.y == playerPosY:
            return True;
        else:
            return False;

    def setBomb(self, playerPosX, playerPosY):
        self.x = playerPosX
        self.y = playerPosY
        # set to data and render
        # start loop

    def bombTimer(self):
        # start the timer
        # blink

    def explodeBomb(self):
        # 


if __name__ == "__main__":
    m = Move()
    m.curses_loop()
