# -*- coding: utf-8 -*-

import pyaudio
import wave
import sys
from pylibpd import *
import array
import select

p = pyaudio.PyAudio()

ch = 1
sr = 11025
tpb = 16
bs = libpd_blocksize()

fpb =  bs * tpb

stream = p.open(format = pyaudio.paInt16,
                channels = ch,
                rate = sr,
                output = True,
                frames_per_buffer = fpb)

m = PdManager(0, ch, sr, tpb)
#libpd_open_patch('../libpd/python/bloopy.pd')
libpd_open_patch('wowhack.pd')

libpd_float('pitch', 40)
libpd_float('volume', 127)

while stream.is_active():
    ready = select.select([sys.stdin], [], [], 0)[0]
    if ready:
        line = sys.stdin.readline()
        commands = line.split()
        if len(commands) == 2:
            try:
               pitch = float(commands[0])
               volume = float(commands[1])
               print pitch, volume
               libpd_float('pitch', pitch)
               libpd_float('volume', volume)
            except ValueError:
                pass
    else:
        inbuf = array.array('h', '\x00\x00' * fpb)
        outbuf = array.array('h', '\x00\xff' * fpb)
        libpd_process_short(tpb, inbuf, outbuf)
        stream.write(outbuf)

stream.close()
p.terminate()
libpd_release()
