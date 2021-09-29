import matplotlib.pyplot as plt
import pandas as pd
import os
full_path = os.path.realpath(__file__)

Z = [5, 25, 50]
K = [5, 15, 25]
D = ["RunTimeSeriesRANDOM"]
for data_type in D:
    for l in Z:
        for k in K:
            algotype1 = "SL"
            algotype = "CL"
            pathKL = os.getcwd()+"/zeitreihenClustering/results/runTime/results/" + \
                data_type+algotype1+"_ZR"+str(l)+"k"+str(k)+".txt"
            df = pd.read_csv(
                pathKL, skiprows=6, sep=",", names=["n", "m", "k", "time", "median", "min", "max", "freq"])
            plt.scatter(df["n"]*df["m"], df["time"], s=10, label=algotype1)

            pathKL = os.getcwd()+"/zeitreihenClustering/results/runTime/results/" + \
                data_type+algotype+"_ZR"+str(l)+"k"+str(k)+".txt"
            df = pd.read_csv(
                pathKL, skiprows=6, sep=",", names=["n", "m", "k", "time", "median", "min", "max", "freq"])
            plt.scatter(df["n"]*df["m"], df["time"], s=10, label=algotype)

            plt.legend(loc='upper left')
            plt.xlabel("Knotenanzahl V")
            plt.ylabel("Zeit")
            plt.title("Vergleich CL mit SL k="+str(k) +
                      " zeitreihenlänge="+str(l))

            path_save = os.path.dirname(full_path)+"/diagramm/compare/klassisch/" + \
                data_type + "compareKlassisch_"+str(l)+"k"+str(k)+".png"
            plt.savefig(path_save, format='png')
            plt.cla()
