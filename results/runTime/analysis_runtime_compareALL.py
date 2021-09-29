import matplotlib.pyplot as plt 
import pandas as pd
import os
full_path = os.path.realpath(__file__)

Z = [5,25,50]
K = [5, 15,25]
A = ["SL","CL"]
D = ["RunTimeSeriesRANDOM"]
for data_type in D:
    for l in Z:
        for k in K:
            for algotype in A: 
                pathKL=os.getcwd()+"/zeitreihenClustering/results/runTime/results/"+data_type+algotype+"_ZR"+str(l)+"k"+str(k)+".txt"
                df = pd.read_csv(
                pathKL,skiprows=6, sep=",",names=["n","m","k","time","median","min","max","freq"])
                plt.scatter(df["n"]*df["m"],df["time"], s=10, label=algotype)

                pathKL=os.getcwd()+"/zeitreihenClustering/results/runTime/results/"+data_type+algotype+"FDN_Eff_ZR"+str(l)+"k"+str(k)+".txt"
                df = pd.read_csv(
                pathKL,skiprows=6, sep=",",names=["n","m","k","time","median","min","max","freq"])
                plt.scatter(df["n"]*df["m"],df["time"], s=10, label=algotype+"FDN")

                pathKL=os.getcwd()+"/zeitreihenClustering/results/runTime/results/"+data_type+algotype+"DN_ZR"+str(l)+"k"+str(k)+".txt"
                df = pd.read_csv(
                pathKL,skiprows=6, sep=",",names=["n","m","k","time","median","min","max","freq"])
                plt.scatter(df["n"]*df["m"],df["time"], s=10, label=algotype+"DN")

                plt.legend(loc='upper left')
                plt.xlabel("Knotenanzahl V")
                plt.ylabel("Zeit")
                plt.title("Vergleich "+algotype+" k="+str(k)+" zeitreihenlänge="+str(l)+" mit FDNEff")

                path_save=os.path.dirname(full_path)+"/diagramm/compare/ALL/"+data_type+algotype+"compareEff_l"+str(l)+"k"+str(k)+"N.png"
                plt.savefig(path_save, format='png')
                plt.cla()

            for algotype in A:
                pathKL=os.getcwd()+"/zeitreihenClustering/results/runTime/results/"+data_type+algotype+"_ZR"+str(l)+"k"+str(k)+".txt"
                df = pd.read_csv(
                pathKL,skiprows=6, sep=",",names=["n","m","k","time","median","min","max","freq"])
                plt.scatter(df["n"]*df["m"],df["time"], s=10, label=algotype)

                pathKL=os.getcwd()+"/zeitreihenClustering/results/runTime/results/"+data_type+algotype+"FDN_NOTEff_ZR"+str(l)+"k"+str(k)+".txt"
                df = pd.read_csv(
                pathKL,skiprows=6, sep=",",names=["n","m","k","time","median","min","max","freq"])
                plt.scatter(df["n"]*df["m"],df["time"], s=10, label=algotype+"FDN")

                pathKL=os.getcwd()+"/zeitreihenClustering/results/runTime/results/"+data_type+algotype+"DN_ZR"+str(l)+"k"+str(k)+".txt"
                df = pd.read_csv(
                pathKL,skiprows=6, sep=",",names=["n","m","k","time","median","min","max","freq"])
                plt.scatter(df["n"]*df["m"],df["time"], s=10, label=algotype+"DN")

                plt.legend(loc='upper left')
                plt.xlabel("Knotenanzahl V")
                plt.ylabel("Zeit")
                plt.title("Vergleich"+algotype+" k="+str(k)+" zeitreihenlänge="+str(l)+" mit FDN_NOTEff")

                path_save=os.path.dirname(full_path)+"/diagramm/compare/ALL/"+data_type+algotype+"compareNOTEff_l"+str(l)+"k"+str(k)+"N.png"
                plt.savefig(path_save, format='png')
                plt.cla()
