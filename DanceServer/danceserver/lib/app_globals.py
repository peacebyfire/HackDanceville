# -*- coding: utf-8 -*-

"""The application's Globals object"""
from tg import app_globals as g

from hackdanceville.animation import Animator
from hackdanceville.queue import DancefloorLoop

__all__ = ['Globals']


class LoopInterface(object):
    def __init__(self, cls, **kwargs):
        self._loop = None
        self._loop_kwargs = kwargs
        self.cls = cls

    @property
    def loop(self):
        if self._loop is None:
            self.load_loop()
        return self._loop

    def load_loop(self, data=None):
        if data:
            self._loop_kwargs["data"] = data
        self._loop = self.cls(g.dancefloor, **self._loop_kwargs)

    def put(self, data):
        if self.loop.is_alive():
            self.loop.queue.put(data)
        else:
            self.start(data)

    def start(self, data=None):
        try:
            if data:
                self.loop.queue.put(data)
            self.loop.start()
        except RuntimeError:
            self.load_loop(data=data)
            self.loop.start()

    def kill(self):
        if self._loop and self._loop.is_alive():
            self.loop.kill()
            self._loop = None


class Globals(object):
    """Container for objects available throughout the life of the application.

    One instance of Globals is created during application initialization and
    is available during requests via the 'app_globals' variable.

    """

    def __init__(self):
        self.static_loop = LoopInterface(DancefloorLoop)
        self.animation_loop = LoopInterface(Animator)

    def put_static_data(self, data):
        self.animation_loop.kill()
        self.static_loop.put(data)

    def put_animation_data(self, data):
        self.static_loop.kill()
        self.animation_loop.put(data)
