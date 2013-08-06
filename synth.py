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

def pd_receive(*s):
  print 'received:', s

libpd_set_print_callback(pd_receive)
libpd_set_float_callback(pd_receive)
libpd_set_list_callback(pd_receive)
libpd_set_symbol_callback(pd_receive)
libpd_set_noteon_callback(pd_receive)

libpd_subscribe('real')
libpd_subscribe('complex')

#libpd_open_patch('../libpd/python/bloopy.pd')
libpd_open_patch('wowhack.pd')

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
