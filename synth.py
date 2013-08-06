# -*- coding: utf-8 -*-

import pyaudio
import wave
import sys
from pylibpd import *
import array
import select

p = pyaudio.PyAudio()

ch = 1
sr = 44100
tpb = 1
bs = libpd_blocksize()

stream = p.open(format = pyaudio.paInt16,
                channels = ch,
                rate = sr,
                output = True,
                frames_per_buffer = bs * tpb)

m = PdManager(0, ch, sr, 1)
#libpd_open_patch('../libpd/python/bloopy.pd')
libpd_open_patch('wowhack.pd')

dummy = array.array('h', '\x00\x00' * ch * bs)

libpd_float('pitch', 40)
libpd_float('volume', 127)


while True:
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
        outp = m.process(dummy)
        stream.write(outp, len(outp))

stream.close()
p.terminate()
libpd_release()
