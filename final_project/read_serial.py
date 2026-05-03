import serial
import time
import matplotlib.pyplot as plt
import pandas as pd

com = serial.Serial("COM3", 115200)

file_name = "measurements/heated_measurements.csv"

with open(file_name, mode="+w") as f:
    f.write("time,bit,reading\n")

start_time = time.localtime(time.time())
cur_time = time.localtime(time.time())

start_time_sec = time.time()

duration = 0.01
while cur_time.tm_sec - start_time.tm_sec < duration:
    with open(file_name, mode="+a") as f:
        line = com.readline()
        line = str(line, encoding="utf-8")
        diff = time.time() - start_time_sec
        line = str(diff) + "," + line
        f.write(line)
    
    cur_time = time.localtime(time.time())

