from tg import expose, app_globals as g
from danceserver.lib.base import BasePluginController
from hackdanceville.move import SingleKeyboardBomberman


class GifUploadController(BasePluginController):

    def __init__(self, api):
        super(GifUploadController, self).__init__(api)
        self.gifupload = None

    @expose('danceserver.plugins.gifupload.templates.index')
    def index(self):
        return {}

