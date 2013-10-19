import curses

from hackdanceville.queue import DancefloorLoop


def movement_wrapper(func):
    def wrapper(inst, *args, **kwargs):
        inst.data[inst.y * 8 + inst.x][inst.color_i] = 0
        func(inst, *args, **kwargs)
        inst.data[inst.y * 8 + inst.x][inst.color_i] = 255
    return wrapper


class Move(object):

    def __init__(self):
        self.init_data()
        self.keymap = {
            curses.KEY_UP: self.move_up,
            curses.KEY_DOWN: self.move_down,
            curses.KEY_LEFT: self.move_left,
            curses.KEY_RIGHT: self.move_right,
            99: self.change_color,
            113: self.quit
        }
        self.color_i = 1
        self.loop = None
        self.go = False

    def init_data(self):
        self.data = [[0, 255, 0]]
        for i in xrange(63):
            self.data.append([0, 0, 0])
        self.x = 0
        self.y = 0

    @movement_wrapper
    def move_left(self):
        self.x = (self.x - 1) % 8

    @movement_wrapper
    def move_right(self):
        self.x = (self.x + 1) % 8

    @movement_wrapper
    def move_up(self):
        self.y = (self.y - 1) % 8

    @movement_wrapper
    def move_down(self):
        self.y = (self.y + 1) % 8

    @movement_wrapper
    def change_color(self):
        self.color_i = 2 if self.color_i == 1 else 1

    def initialize_loop(self):
        if not self.loop or not self.loop.is_alive():
            self.loop = DancefloorLoop(data=self.data)
            self.loop.start()
            self.go = True

    def quit(self):
        self.data = None
        self.go = False

    def put(self, key):
        print 'putting ' + str(key)
        if key in self.keymap:
            func = self.keymap[key]
            func()
            print 'ran ' + str(func)
        self.loop.queue.put(self.data)

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


if __name__ == "__main__":
    m = Move()
    m.curses_loop()
