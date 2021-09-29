import matplotlib.pyplot as plt
import pandas as pd
import os
full_path = os.path.realpath(__file__)

zeitreihe = [5, 25, 50]
cluster = [5, 15, 25]
A = ["SL", "SLDN", "CL", "CLDN"]
S = ["", "OHNEinitial"]
D = ["RunTimeSeriesRANDOM"]
for data_type in D:
    for algotype in A:
        for s in S:
            for l in zeitreihe:
                for k in cluster:
                    path = os.getcwd()+"/zeitreihenClustering/results/runTime/results/" + \
                        data_type+algotype+"_ZR"+str(l)+"k"+str(k)+s+".txt"
                    path_save = os.path.dirname(full_path)+"/diagramm/runTimealgo/" + \
                        data_type+algotype+s+"gesamt.png"
                    df = pd.read_csv(
                        path, skiprows=6,  sep=",", names=["n", "m", "k", "time", "median", "min", "max", "freq"])
                    print(path)
                    print(df)
                    plt.scatter(df["n"]*df["m"], df["time"], s=10,
                                label="k="+str(k)+" l="+str(l))
                    plt.legend(loc='upper left')
                    plt.xlabel("Knotenanzahl V")
                    plt.ylabel("Zeit")
                    if algotype =="SLDN" and s=="OHNEinitial":
                        dummy="    "
                    if algotype !="SLDN":
                        dummy=""
                    if s == "":
                        plt.title("Gesamt Laufzeit "+algotype)
                    if s == "OHNEinitial":
                        plt.title(dummy+"Gesamt Laufzeit ohne Frechet Berechnungem "+algotype)
            plt.savefig(path_save, format='png')
            plt.cla()

A = ["SLFDN", "CLFDN"]
S = ["_NOTEff", "_Eff"]
for data_type in D:
    for algotype in A:
        for s in S:
            for l in zeitreihe:
                for k in cluster:
                    path = os.getcwd()+"/zeitreihenClustering/results/runTime/results/" + \
                        data_type+algotype+s+"_ZR"+str(l)+"k"+str(k)+".txt"
                    path_save = os.path.dirname(full_path)+"/diagramm/runTimealgo/" + \
                        data_type+algotype+s+"gesamt.png"
                    df = pd.read_csv(
                        path, skiprows=6,  sep=",", names=["n", "m", "k", "time", "median", "min", "max", "freq"])
                    print(path)
                    print(df)
                    plt.scatter(df["n"]*df["m"], df["time"], s=10,
                                label="k="+str(k)+" l="+str(l))
                    plt.legend(loc='upper left')
                    plt.xlabel("Knotenanzahl V")
                    plt.ylabel("Zeit")
                    plt.title("Gesamt Laufzeit "+algotype+s)
            plt.savefig(path_save, format='png')
            plt.cla()
