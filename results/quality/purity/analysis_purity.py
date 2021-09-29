import matplotlib.pyplot as plt
import pandas as pd
import os
full_path = os.path.realpath(__file__)

gitter = [4, 8, 12, 16, 20]
T = ["SL", "CL"]
for type in T:
    for dim in gitter:
        path = os.getcwd()+"/zeitreihenClustering/results/quality/results/purity" + \
            type+"_n"+str(dim)+".txt"
        path_saveABS = os.path.dirname(
            full_path) + "/diagramm/purity"+type+"_n"+str(dim)+"absolut.png"
        df = pd.read_csv(
            path, skiprows=4, sep=",", names=["n", "a", "AVG "+type, "AVG "+type+"FDN", "AVG "+type+"DN", "AVG "+type+"FDN/"+type, "AVG "+type+"DN/"+type])
        print(df)

        plt.cla()
        plt.plot(df["a"], df["AVG "+type+"FDN"], label="AVG "+type+"FDN")
        plt.plot(df["a"], df["AVG "+type+"DN"], label="AVG "+type+"DN")
        plt.plot(df["a"], df["AVG "+type], label="AVG "+type)

        plt.legend(loc='lower left')
        plt.xlabel("Amplitude a")
        plt.title("Purity "+type +
                  " auf "+str(dim) + "x"+str(dim))
        plt.savefig(path_saveABS, format='png')

        plt.cla()
