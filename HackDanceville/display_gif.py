import os, os.path
from hackdanceville.api import dancefloor
from PIL import Image, ImagePalette
from time import sleep
import re

## break gif into frames
#im = Image.open('../images/crazySquares.gif')
def iter_frames(im):
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

def display_gif(filename, frame_delay):
    im = Image.open(filename)
    myFrames = []
    for i, frame in enumerate(iter_frames(im)):
        #frame.save('test%d.png' % i,**frame.info)
        frame.resize((8,8))
        im = frame.convert('RGB')
        pix = im.load()
        picture = [im.getpixel((i, j)) for i in range(8) for j in range(8)]
        sleep(frame_delay)
        dancefloor.send(picture)
