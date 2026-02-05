from pathlib import Path
import ipaddress

def count_ips_per_network():
    networks = {}
    with open("subnets.txt") as f:
        for line in f:
            line = line.split("|")
            if len(line) < 3:
                continue

            network_name = line[2]
            networks[network_name] = networks.get(network_name, 0) + 1
    sort_networks = dict(sorted(networks.items(), key = lambda item: item[1], reverse=True))
    print(sort_networks)
    with open("network_count.txt", "w") as f:
        for key in sort_networks.keys():

            f.write(str(sort_networks[key]) + " " + key)
        
def get_all_amazon_ips():
    ip_addresses = []
    with open("subnets.txt") as f:
        for line in f:
            line = line.split("|")

            for i in range(len(line)):
                line[i] = line[i].strip()

            if len(line) < 3 or line[0] == "NA":
                continue 

            if int(line[0]) == 16509:
                interface = ipaddress.IPv4Interface(line[1])
                ip_addresses.append(str(interface.with_prefixlen))

    with open("amazon_ip_addresses.txt", "w") as f:
        for ip in ip_addresses:
            f.write(ip + "\n")

def both_ports_open():
    port_22 = set()
    with open("amazon_probe_port22.txt") as f:
        for line in f:
            port_22.add(line)

    port_443 = set()
    with open("amazon_probe_port443.txt") as f:
        for line in f:
            port_443.add(line)

    both_open = port_22 & port_443

    with open("amazon_both_ports_open.txt", "w") as f:
        for elem in both_open:
            f.write(elem)


#get_all_amazon_ips()
both_ports_open()
