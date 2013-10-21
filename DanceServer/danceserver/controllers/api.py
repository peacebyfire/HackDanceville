import json

from tg import expose, app_globals as g, request

from danceserver.lib.base import BaseController
from hackdanceville.freq import json_data_to_floor_array_shuffled


class APIController(BaseController):
    @expose('json')
    def flash(self, data):
        data = json.loads(data)
        g.dancefloor.send(data)

    @expose('json')
    def display(self, data):
        data = json.loads(data)
        g.put_static_data(data)

    @expose('json')
    def histogram(self, data):
        data = json.loads(data)

    @expose('json')
    def scaled(self, data=None, **kw):
        if data is None:
            data = request.body
        print data
        data = json.loads(data)
        converted = json_data_to_floor_array_shuffled(data, 8, 8)
        print converted
        g.put_static_data(converted)

    @expose()
    def stop(self):
        g.static_loop.kill()
        g.animation_loop.kill()
        return ''