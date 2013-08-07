# -*- coding: utf-8 -*-

import wave
import sys
from pylibpd import *
import array
import select

import alsaaudio
import pyaudio

class AudioPyAudio(object):
  def __init__(self, samplerate, channels, frames_per_buffer):
    self.p = pyaudio.PyAudio()
    self.stream = self.p.open(format = pyaudio.paInt16,
                              channels = channels,
                              rate = samplerate,
                              output = True,
                              frames_per_buffer = frames_per_buffer)

  def write(self, buf, frames):
    self.stream.write(buf, frames)

class AudioAlsa(object):
  def __init__(self, samplerate, channels, frames_per_buffer):
    self.device = alsaaudio.PCM(card='default')
    self.device.setchannels(channels)
    self.device.setrate(samplerate)

  def write(self, buf, frames):
    self.device.write(buf.tostring())

ch = 1
sr = 44100 #11025
tpb = 8
bs = libpd_blocksize()

fpb =  bs * tpb

m = PdManager(0, ch, sr, tpb)

#audio = AudioPyAudio(sr, ch, fpb)
audio = AudioAlsa(sr, ch, fpb)

def pd_receive(*s):
  if s[0] == 'onesec':
    fft = array.array('f', [0.0] * 64)
    libpd_read_array(fft, 'fft_array', 0, 64)
    print(" ".join([str(a) for a in fft[0:40]]))

#libpd_set_print_callback(pd_receive)
libpd_set_bang_callback(pd_receive)
libpd_set_message_callback(pd_receive)
libpd_set_float_callback(pd_receive)
libpd_set_list_callback(pd_receive)
libpd_set_symbol_callback(pd_receive)
libpd_set_noteon_callback(pd_receive)

libpd_subscribe('onesec')

#libpd_open_patch('../libpd/python/bloopy.pd')
libpd_open_patch('wowhack.pd')

while True:
    ready = select.select([sys.stdin], [], [], 0)[0]
    if ready:
        line = sys.stdin.readline()
        commands = line.split()
        if len(commands) == 2:
            try:
               pitch = float(commands[0])
               volume = float(commands[1])
               libpd_float('pitch', pitch)
               libpd_float('volume', volume)
            except ValueError:
                pass
    else:
        inbuf = array.array('h', '\x00\x00' * fpb)
        outbuf = array.array('h', '\x00\xff' * fpb)
        libpd_process_short(tpb, inbuf, outbuf)
        audio.write(outbuf, len(outbuf))

stream.close()
p.terminate()
libpd_release()
