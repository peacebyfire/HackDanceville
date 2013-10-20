import json
from tg import expose, app_globals as g

from hackdanceville.animation import Animator
from danceserver.lib.base import BasePluginController


class BuilderController(BasePluginController):

    def __init__(self, api):
        super(BuilderController, self).__init__(api)
        self.animator = Animator(self.api)

    @expose('danceserver.plugins.builder.templates.index')
    def index(self):
        return {}

    @expose('json')
    def display(self, data):
        data = json.loads(data)
        g.put_animation_data(data)
        return {"success": True}

    @expose('json')
    def stop(self):
        g.animation_loop.kill()
        return {"success": True}
