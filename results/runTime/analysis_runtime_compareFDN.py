import matplotlib.pyplot as plt 
import pandas as pd
import os
full_path = os.path.realpath(__file__)

Z = [5,25,50]
K = [5,15,25]
algotype1="SLFDN"
algotype="CLFDN"
S=["_","_NOT"]
D = ["RunTimeSeriesRANDOM"]
for data_type in D:
    for k in K:
        for l in Z:
            for s in S:
                pathKL=os.getcwd()+"/zeitreihenClustering/results/runTime/results/"+data_type+algotype1+s+"Eff_ZR"+str(l)+"k"+str(k)+".txt"
                df = pd.read_csv(
                pathKL,skiprows=6, sep=",",names=["n","m","k","time","median","min","max","freq"])
                plt.scatter(df["n"]*df["m"],df["time"], s=10, label=algotype1)

                pathKL=os.getcwd()+"/zeitreihenClustering/results/runTime/results/"+data_type+algotype+s+"Eff_ZR"+str(l)+"k"+str(k)+".txt"
                df = pd.read_csv(
                pathKL,skiprows=6, sep=",",names=["n","m","k","time","median","min","max","freq"])
                plt.scatter(df["n"]*df["m"],df["time"], s=10, label=algotype)

                plt.legend(loc='upper left')
                plt.xlabel("Knotenanzahl V")
                plt.ylabel("Zeit")
                plt.title("Vergleich CLFDN und SLFDN k="+str(k)+" zeitreihenlänge="+str(l))

                path_save=os.path.dirname(full_path)+"/diagramm/compare/FDN/"+data_type+"compare"+s+"Eff_CLFDN_SLFDN_"+str(l)+"k"+str(k)+".png"
                plt.savefig(path_save, format='png')
                plt.cla()

for k in K:
    for l in Z:
        for data_type in D:
            for s in S:
                pathKL=os.getcwd()+"/zeitreihenClustering/results/runTime/results/"+data_type+algotype1+s+"Eff_ZR"+str(l)+"k"+str(k)+".txt"
                df = pd.read_csv(
                pathKL,skiprows=6, sep=",",names=["n","m","k","time","median","min","max","freq"])
                plt.scatter(df["n"]*df["m"],df["freq"], s=10, label=algotype1)

                pathKL=os.getcwd()+"/zeitreihenClustering/results/runTime/results/"+data_type+algotype+s+"Eff_ZR"+str(l)+"k"+str(k)+".txt"
                df = pd.read_csv(
                pathKL,skiprows=6, sep=",",names=["n","m","k","time","median","min","max","freq"])
                plt.scatter(df["n"]*df["m"],df["freq"], s=10, label=algotype)

                plt.legend(loc='upper left')
                plt.xlabel("Knotenanzahl V")
                plt.ylabel("Anzahl Frechet Berechnungen")
                plt.title("Vergleich CLFDN und SLFDN k="+str(k)+" zeitreihenlänge="+str(l))

                path_save=os.path.dirname(full_path)+"/diagramm/compare/FDN/"+data_type+"compare"+s+"Eff_CLFDN_SLFDN_"+str(l)+"k"+str(k)+"freq.png"
                plt.savefig(path_save, format='png')
                plt.cla()
