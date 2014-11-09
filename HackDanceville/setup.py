# -*- coding: utf-8 -*-
try:
    from setuptools import setup, find_packages
except ImportError:
    from ez_setup import use_setuptools
    use_setuptools()
    from setuptools import setup, find_packages

setup(
    name='HackDanceville',
    version='0.1',
    description='Hack til you drop',
    author_email='',
    url='',
    packages=find_packages(exclude=['ez_setup']),
    include_package_data=True,
)
