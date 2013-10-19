# -*- coding: utf-8 -*-
"""
Global configuration file for TG2-specific settings in DanceServer.

This file complements development/deployment.ini.

"""
from tg import config
from tg.configuration import AppConfig

from hackdanceville.api import DancefloorAPI
import danceserver
from danceserver import model
from danceserver.lib import app_globals, helpers


class DanceConfig(AppConfig):

    def __init__(self, minimal=False, root_controller=None):
        super(DanceConfig, self).__init__(minimal, root_controller)

        self.renderers = ['json', 'jinja']
        self.package = danceserver
        self.default_renderer = 'jinja'
        self.jinja_extensions = ['jinja2.ext.with_']
        self.use_sqlalchemy = False
        self.use_ming = False
        self.use_transaction_manager = False
        self.auth_backend = None
        self.use_toscawidgets = False

    def after_init_config(self):
        config['package'] = self.package

    def setup_helpers_and_globals(self):
        super(DanceConfig, self).setup_helpers_and_globals()
        self.setup_dancefloor()

    def setup_dancefloor(self):
        dancefloor = DancefloorAPI(
            config['dancefloor.ip'], int(config['dancefloor.port']))
        config['pylons.app_globals'].dancefloor = dancefloor


base_config = DanceConfig()
