#!/usr/bin/env python
# -*- coding: utf-8 -*-

import re
import sys
import time


def wifi_sample(interface):
    with open('/proc/net/wireless') as f:
        for line in f:
            if interface + ":" in line:
                return line


def wifi_stats(interface):
    sample = wifi_sample(interface) or [0, 0, 0]
    return [int(i[:-1]) for i in re.split("\s+", sample)[3:6]]


interfaces = sys.argv[1:]
while True:
    signals = []
    for interface in interfaces:
        signals.append(wifi_stats(interface)[1])

    print("\t".join([str(i) for i in signals]))
    sys.stdout.flush()
    time.sleep(0.5)
