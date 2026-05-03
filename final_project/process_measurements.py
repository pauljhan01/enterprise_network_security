import matplotlib.pyplot as plt
import pandas as pd

def retrieve_data(source : str):
    df = pd.read_csv(source)
    df = df.loc[:100, :]
    return df

def plot_heat_data(data : pd.DataFrame, dest : str):
    plt.plot(data["time"], data["reading"])
    plt.xlabel("Time")
    plt.ylabel("Temperature Values")
    plt.savefig(dest)
    plt.close()

def plot_bit_data(data : pd.DataFrame, dest : str):
    plt.plot(data["time"], data["bit"])
    plt.xlabel("Time")
    plt.ylabel("Bit Value")
    plt.savefig(dest)
    plt.close()

def extract_bit_sequence(data : pd.DataFrame, dest : str):
    bit_sequence = data["bit"][:]
    with open(dest, mode="+w") as f:
        f.write(bit_sequence.to_string(index=False))

if __name__ == "__main__":
    source_name = "measurements/heated_measurements.csv"
    figure_dest = "graphs/heated_heat_data.png"
    bit_figure_dest = "graphs/heated_bit_heat_data.png"
    bit_seq_dest = "bit_sequence/heated_bit_sequence.txt"

    heat_data = retrieve_data(source=source_name)
    plot_heat_data(heat_data, figure_dest)
    plot_bit_data(heat_data, bit_figure_dest)
    extract_bit_sequence(heat_data, bit_seq_dest)

