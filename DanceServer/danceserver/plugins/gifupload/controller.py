import os
from tg import expose, app_globals as g
import string
from danceserver.lib.base import BasePluginController
from hackdanceville.move import SingleKeyboardBomberman
import Image, ImagePalette
import re


class GifUploadController(BasePluginController):

    def __init__(self, api):
        super(GifUploadController, self).__init__(api)
        self.gifupload = None

    @expose('danceserver.plugins.gifupload.templates.index')
    def index(self):
        return {}

    @expose('danceserver.plugins.gifupload.templates.index')
    def upload(self, yourgif, framerate):
        self.display_gif(yourgif.file, int(framerate))
        return {} 

    def iter_frames(self, im):
        try:
            i= 0
            while 1:
                im.seek(i)
                imframe = im.copy()
                if i == 0: 
                    palette = imframe.getpalette()
                else:
                    imframe.putpalette(palette)
                yield imframe
                i += 1
        except EOFError:
            pass

    def display_gif(self, file, frame_delay):
        im = Image.open(file)
        myFrames = []
        for i, frame in enumerate(self.iter_frames(im)):
            #frame.save('test%d.png' % i,**frame.info)
            frame.resize((8,8))
            im = frame.convert('RGB')
            pix = im.load()
            for i in range(frame_delay):
                myFrames.append([im.getpixel((i, j)) for i in range(8) for j in range(8)])

        print myFrames
        g.put_animation_data(myFrames)
