# -*- coding: utf-8 -*-

import pyaudio
import wave
import sys
from pylibpd import *
import array
import select

p = pyaudio.PyAudio()

ch = 1
sr = 48000
tpb = 64
bs = libpd_blocksize()

fpb =  bs * tpb

stream = p.open(format = pyaudio.paInt16,
                channels = ch,
                rate = sr,
                output = True,
                frames_per_buffer = 48000)

m = PdManager(0, ch, sr, tpb)
#libpd_open_patch('../libpd/python/bloopy.pd')
libpd_open_patch('wowhack.pd')

libpd_float('pitch', 40)
libpd_float('volume', 127)

inbuf = array.array('h', '\x00\x00' * ch * fpb)
outbuf = array.array('h', '\x00\x00' * ch * fpb)

while stream.is_active():
    ready = select.select([sys.stdin], [], [], 0)[0]
    if ready:
        line = sys.stdin.readline()
        commands = line.split()
        if len(commands) == 2:
            if commands[0] in ['pitch', 'volume']:
                try:
                    val = float(commands[1])
                    libpd_float(commands[0], val)
                except ValueError:
                    pass
    else:
        libpd_process_short(tpb, inbuf, outbuf)
        stream.write(outbuf, fpb)

stream.close()
p.terminate()
libpd_release()
