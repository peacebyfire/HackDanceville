from tg import expose, app_globals as g
from danceserver.lib.base import BasePluginController
from hackdanceville.move import SingleKeyboardBomberman


class BombermanController(BasePluginController):

    def __init__(self, api):
        super(BombermanController, self).__init__(api)
        self.bomber = None

    @expose('danceserver.plugins.bomberman.templates.index')
    def index(self):
        return {}

    @expose('danceserver.plugins.bomberman.templates.local')
    def local(self):
        g.static_loop.kill()
        g.animation_loop.kill()
        if self.bomber:
            self.bomber.kill()
        self.bomber = SingleKeyboardBomberman(api=self.api)
        self.bomber.start()
        return {}

    @expose('json')
    def keypress(self, key):
        if self.bomber:
            key = int(key)
            self.bomber.put(key)
            if self.bomber.dead:
                self.bomber = None
                return {"status": 'gameover'}
            return {"data": self.bomber.cur_map}
