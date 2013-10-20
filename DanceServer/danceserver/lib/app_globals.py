# -*- coding: utf-8 -*-

"""The application's Globals object"""

__all__ = ['Globals']


class Globals(object):
    """Container for objects available throughout the life of the application.

    One instance of Globals is created during application initialization and
    is available during requests via the 'app_globals' variable.

    """

    def __init__(self):
        self.loops = {}

    def add_loop(self, key, loop):
        self.loops.setdefault(key, []).append(loop)

    def kill_loops(self):
        for key, loops in self.loops.iteritems():
            for loop in loops:
                loop.kill()
        self.loops = {}

    def kill_loops_not_in(self, key):
        for k, loops in self.loops.iteritems():
            if k != key:
                for loop in loops:
                    loop.kill()
                self.loops[k] = []
