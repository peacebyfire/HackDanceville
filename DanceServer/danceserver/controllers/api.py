import json

from tg import expose, app_globals as g

from danceserver.lib.base import BaseController


class APIController(BaseController):
    def _before(self):
        g.kill_all_loops()

    @expose('json')
    def flash(self, data):
        data = json.loads(data)
        g.dancefloor.send(data)

    @expose('json')
    def display(self, data):
        data = json.loads(data)
        g.put_static_data(data, delay=0.5)

    @expose('json')
    def histogram(self, data):
        data = json.loads(data)

    @expose('json')
    def scaled(self, data):
        data = json.loads(data)
