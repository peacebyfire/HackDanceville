# -*- coding: utf-8 -*-
"""Main Controller"""
import pkg_resources

from tg import expose, app_globals
from tg.i18n import ugettext as _, lazy_ugettext as l_
from danceserver.controllers.api import APIController

from danceserver.lib.base import BaseController
from danceserver.controllers.error import ErrorController

__all__ = ['RootController']


class RootController(BaseController):
    """
    The root controller for the DanceServer application.

    All the other controllers and WSGI applications should be mounted on this
    controller. For example::

        panel = ControlPanelController()
        another_app = AnotherWSGIApplication()

    Keep in mind that WSGI applications shouldn't be mounted directly: They
    must be wrapped around with :class:`tg.controllers.WSGIAppController`.

    """

    error = ErrorController()
    api = APIController()

    def __init__(self):
        self.plugins = []
        for ep in pkg_resources.iter_entry_points('dancefloor.plugins'):
            controller = ep.load()(app_globals.dancefloor)
            setattr(self, ep.name, controller)
            self.plugins.append({
                "url": ep.name,
                "label": getattr(controller, 'label', ep.name.capitalize())
            })

    @expose('danceserver.templates.index')
    def index(self):
        return {
            "plugins": self.plugins
        }
