from tg import expose, app_globals as g
from danceserver.lib.base import BasePluginController
from hackdanceville.move import Move


class BombermanController(BasePluginController):

    def __init__(self, api):
        super(BombermanController, self).__init__(api)
        self.bomber = Move()

    @expose('danceserver.plugins.bomberman.templates.index')
    def index(self):
        return {}

    @expose()
    def keypress(self, key):
        key = int(key)
        self.bomber.put(key)
        if not self.bomber.go:
            return 'gameover'
