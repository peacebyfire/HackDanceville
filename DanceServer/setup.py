# -*- coding: utf-8 -*-

try:
    import logging
    import multiprocessing
except:
    pass

import sys

py_version = sys.version_info[:2]

try:
    from setuptools import setup, find_packages
except ImportError:
    from ez_setup import use_setuptools

    use_setuptools()
    from setuptools import setup, find_packages

testpkgs = ['WebTest >= 1.2.3', 'nose', 'coverage']

install_requires = [
    "TurboGears2 >= 2.3.0b3",
    "Babel",
]
if py_version == (3, 2):
    #jinja2 2.7 is incompatible with Python 3.2
    install_requires.append('jinja2 < 2.7')
else:
    install_requires.append('jinja2')

setup(
    name='DanceServer',
    version='0.1',
    description='',
    author='',
    author_email='',
    #url='',
    packages=find_packages(exclude=['ez_setup']),
    install_requires=install_requires,
    include_package_data=True,
    test_suite='nose.collector',
    tests_require=testpkgs,
    package_data={'danceserver': ['i18n/*/LC_MESSAGES/*.mo',
                                  'templates/*/*',
                                  'public/*/*']},
    message_extractors={'danceserver': [
        ('**.py', 'python', None),
        ('templates/**.html', 'genshi', None),
        ('public/**', 'ignore', None)]},

    entry_points={
        'paste.app_factory': [
            'main = danceserver.config.middleware:make_app'
        ],
        'gearbox.plugins': [
            'turbogears-devtools = tg.devtools'
        ],
        'dancefloor.plugins': [
            'builder = danceserver.plugins.builder.controller:BuilderController',
            'bomberman = danceserver.plugins.bomberman.controller:BombermanController'
        ]
    },
    dependency_links=[
        "http://tg.gy/230"
    ],
    zip_safe=False
)
