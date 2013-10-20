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
        self.animator.queue.put(data)
        if not self.animator.is_alive():
            try:
                g.kill_all_loops()
                self.animator.start()
            except RuntimeError:
                self.animator = Animator(self.api, data=data)
                self.animator.start()
            g.add_loop('builder', self.animator)
        return {"success": True}

    @expose('json')
    def stop(self):
        if self.animator.is_alive():
            self.animator.queue.put(None)
            self.animator.join(5)
            self.animator = Animator(self.api)
        return {"success": True}
