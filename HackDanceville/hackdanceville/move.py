import curses

from hackdanceville.queue import dance_queue, DancefloorLoop


class Move(object):

    def __init__(self):
        self.init_data()
        self.keymap = {
            curses.KEY_UP: self.move_up,
            curses.KEY_DOWN: self.move_down,
            curses.KEY_LEFT: self.move_left,
            curses.KEY_RIGHT: self.move_right,
            99: self.change_color
        }
        self.color_i = 1

    def init_data(self):
        self.data = [[0, 255, 0]]
        for i in xrange(63):
            self.data.append([0, 0, 0])
        self.x = 0
        self.y = 0

    def move_left(self):
        self.x = (self.x - 1) % 8

    def move_right(self):
        self.x = (self.x + 1) % 8

    def move_up(self):
        self.y = (self.y - 1) % 8

    def move_down(self):
        self.y = (self.y + 1) % 8

    def change_color(self):
        self.color_i = 2 if self.color_i == 1 else 1

    def start(self):
        stdscr = curses.initscr()
        curses.cbreak()
        stdscr.keypad(1)

        stdscr.addstr(0, 10, "Hit 'q' to quit")
        stdscr.refresh()

        loop = DancefloorLoop(data=self.data)
        loop.start()

        key = ''
        while key != ord('q'):
            key = stdscr.getch()
            stdscr.addch(20, 25, key)
            stdscr.refresh()
            self.data[self.y * 8 + self.x][self.color_i] = 0
            if key in self.keymap:
                func = self.keymap[key]
                func()
            self.data[self.y * 8 + self.x][self.color_i] = 255
            dance_queue.put(self.data)

        dance_queue.put(None)
        curses.endwin()


if __name__ == "__main__":
    m = Move()
    m.start()
