import os
import signal
import subprocess
import time
import matplotlib.pyplot as plt
from pathlib import Path
websites = {"https://en.wikipedia.org/wiki/Cat/":"Cat", "https://en.wikipedia.org/wiki/Dog":"Dog","https://en.wikipedia.org/wiki/Egress_filtering":"Egress", "http://web.mit.edu/":"mit", "http://www.unm.edu/":"umn","https://www.cmu.edu/":"cmu","https://www.berkeley.edu/":"berkeley","https://www.utexas.edu/":"utexas", "https://www.asu.edu/":"asu", "https://www.utdallas.edu/":"utd"}

#websites = {"https://en.wikipedia.org/wiki/Cat/":"Cat","http://web.mit.edu/":"mit"}

def ping_website(browser):
    for website in list(websites.keys()):
        for i in range(5): 
            #name = "tor" + f"_{i}_" + websites[website] + ".pcap"
            name = browser + f"_{i}_" + websites[website] + ".pcap"
            print(name)
            path_to_tcp = "stats1/" + name
            args = "sudo tcpdump -i any -s 0 -nn -w " + path_to_tcp
            args = args.split()
            tcp_process = subprocess.Popen(args=args)

            time.sleep(3)

            #args = "./start-tor-browser.desktop --headless " + website
            args = browser + " --headless " + website
            args = args.split()
            browser_process = subprocess.Popen(args=args)

            time.sleep(15)

#            browser_process.terminate()
            args = "sudo pkill " + browser
            args = args.split()
            subprocess.run(args=args)

            time.sleep(10)

            args = "sudo pkill -f tcpdump"
            args = args.split()
            subprocess.run(args=args)

#            tcp_process.terminate()

            #readable = "tshark -r " + path_to_tcp + " > " + name + ".txt"
            #args = readable.split()
            #subprocess.run(args=args)



def process_txt_for_packet_len():
    path = Path("stats/part2_packet_capture")
    connect = ["firefox", "tor", "vpn"]
    packet_list = dict.fromkeys(connect, {})
    for key in packet_list.keys():
        packet_list[key] = dict.fromkeys(websites.values(), [])

    for entry in path.iterdir():
        if entry.is_file() and ("firefox" in entry.name or "tor" in entry.name or "vpn" in entry.name):
            #args = "tshark -r " + "stats/" + entry.name + " > " + "stats/" + entry.name + ".txt"
            args = "tshark -r " + "stats1/" + entry.name
            args = args.split()
            process = subprocess.run(args=args, capture_output=True)

            with open("stats1/" + entry.name + ".txt", "w") as f:
                f.write(process.stdout.decode("utf-8"))

def process_pcap_for_packets_sent():
    path = Path("stats/part2_packet_capture")
    for entry in path.iterdir():
        if entry.is_file() and ("firefox" in entry.name or "tor" in entry.name or "vpn" in entry.name):
            args = "tshark -r " + "stats/part2_packets_sent/" + entry.name + " -q -z io,stat,0"
            args = args.split()
            process = subprocess.run(args=args, capture_output=True)

            with open("stats/part2_packets_sent/" + entry.name + ".txt", "w") as f:
                f.write(process.stdout.decode("utf-8"))
def process_txt():
    path = Path("stats/part2_packet_length/")
    connect = ["firefox", "tor", "vpn"]
    packet_list = dict.fromkeys(connect, {})
    for key in packet_list.keys():
        packet_list[key] = dict.fromkeys(websites.values(), [])

    packets_sent = dict.fromkeys(connect, {})
    for key in packets_sent.keys():
        packets_sent[key] = dict.fromkeys(websites.values(), [])

    for key in packet_list.keys():
        for entry in path.iterdir():
            if entry.is_file() and ".txt" in entry.name and key in entry.name:
                name = entry.name.split("_")[-1].split(".")[0]
                with open("stats/part2_packet_length/" + entry.name, "r") as f:
                    count = 0
                    for line in f:
                        count += 1
                        line = line.split()
                        for elem in line:
                            if "Len=" in elem:
                                packet_len = int(elem.split("=")[-1])
                                packet_list[key][name].append(packet_len)

                    packets_sent[key][name].append(count)
                    
    avg_packet_sent = dict.fromkeys(connect, {})
    for key in avg_packet_sent.keys():
        avg_packet_sent[key] = dict.fromkeys(websites.values(), 0)

    avg_packet_size = dict.fromkeys(connect, {})                      
    for key in avg_packet_size.keys():
        avg_packet_size[key] = dict.fromkeys(websites.values(), 0)


    for key in avg_packet_size.keys():
        for avg_key in avg_packet_size[key].keys():
            avg_packet_size[key][avg_key] = sum(packet_list[key][avg_key])/len(packet_list[key][avg_key])
            #avg_packet_size[key][avg_key] = sum(packet_list[key][avg_key])/len(packet_list[key][avg_key])

    for key in avg_packet_sent.keys():
        for avg_key in avg_packet_sent[key].keys():
            avg_packet_sent[key][avg_key] = sum(packets_sent[key][avg_key])/len(packets_sent[key][avg_key])

    print(avg_packet_sent)
    print(avg_packet_size)

    return avg_packet_size, avg_packet_sent

def graph_packet_size(packet_size, packet_sent):
    for key in packet_sent.keys():
        plt.scatter([i for i in range(len(packet_sent[key].values()))], packet_sent[key].values(), label=key)
        plt.xticks([i for i in range(len(packet_sent[key].values()))], packet_sent[key].keys())
        plt.ylabel("Avg Packets Sent")
        plt.xlabel("Websites")
        plt.legend()
    plt.savefig("avg_packets_sent.png")

    plt.close()

    for key in packet_size.keys(): 
        plt.scatter([i for i in range(len(packet_size[key].values()))], packet_size[key].values(), label=key)
        plt.xticks([i for i in range(len(packet_size[key].values()))], packet_size[key].keys())
        plt.ylabel("Avg Packet Size")
        plt.xlabel("Websites")
        plt.legend()
    plt.savefig("avg_packets_size.png")

    
#ping_website("firefox")
#process_pcap()
packet_size, packet_sent = process_txt()
#process_pcap_for_packets_sent()
graph_packet_size(packet_size, packet_sent)
