import matplotlib.pyplot as plt
import pandas as pd
import os
#print("Path at terminal when executing this file")
#print(os.getcwd() + "\n")
#print("This file directory only")
full_path = os.path.realpath(__file__)
# print(os.path.dirname(full_path))

M = ["SL", "CL"]
TYPE = ["RealWorld"]
for type in TYPE:
    zr = 15
    path = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/" + \
        type+"_AvgMaxMin_zr"+str(zr)+".txt"
    path_saveAVG = os.path.dirname(
        full_path)+"/diagramm/" + type+"_AvgMaxMin_zr"+str(zr)+".png"
    df = pd.read_csv(
        path, skiprows=5, sep=",", names=["n", "AVG[SL]", "AVG[SLFDN/SL]", "MAX[SLFDN/SL]", "MIN[SLFDN/SL]", "AVG[SLDN/SL]",
                                          "MAX[SLDN/SL]", "MIN[SLDN/SL]", "AVG[CL]", "AVG[CLFDN/CL]", "MAX[CLFDN/CL]",
                                          "MIN[CLFDN/CL]", "AVG[CLDN/CL]",  "MAX[CLDN/CL]", " MIN[CLDN/CL]"])
    print(df)

    plt.plot(df["n"]*df["n"], df["AVG[SLFDN/SL]"],
             marker='o', label="SLFDN/SL")
    plt.plot(df["n"]*df["n"], df["AVG[SLDN/SL]"], marker='o', label="AVG SLDN/SL")
    plt.plot(df["n"]*df["n"], df["AVG[CLFDN/CL]"],
             marker='o', label="CLFDN/CL")
    plt.plot(df["n"]*df["n"], df["AVG[CLDN/CL]"], marker='o', label="AVG CLDN/CL")
    plt.legend(loc='upper left')
    plt.xlabel("|V|")
    plt.title("Durchschnittliche Lösungsqualität mit Daten "+type)
    plt.savefig(path_saveAVG, format='png')
    plt.cla()


TYPE = [1, 2, 4, 8, 16, 32, 64]
for type in TYPE:
    path = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude" + \
        str(type)+".txt"
    path_saveAVG = os.path.dirname(
        full_path)+"/diagramm/kontrollAmplitude_ampl" + str(type)+".png"
    df = pd.read_csv(
        path, sep=",", skiprows=4, names=["n", "a", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
    print(df)
    plt.plot(df["n"]*df["n"], df["AVG[SLFDN/SL]"],
             marker='o', label="AVG SLFDN/SL")
    plt.plot(df["n"]*df["n"], df["AVG[SLDN/SL]"], marker='o', label="AVG SLDN/SL")
    plt.plot(df["n"]*df["n"], df["AVG[CLFDN/CL]"],
             marker='o', label="AVG CLFDN/CL")
    plt.plot(df["n"]*df["n"], df["AVG[CLDN/CL]"], marker='o', label="AVG CLDN/CL")

    plt.legend(loc='upper left')
    plt.xlabel("Knotenanzahl V")
    plt.title("Durchschnittliche Lösungsqualität mit Amplitude "+str(type))
    plt.savefig(path_saveAVG, format='png')
    plt.cla()

M = ["SL", "CL"]
F = ["DN", "FDN"]
for m in M:
    for f in F:
        path1 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude1.txt"
        path2 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude2.txt"
        path4 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude4.txt"
        path8 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude8.txt"
        path16 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude16.txt"
        path32 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude32.txt"
        path64 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude64.txt"
        path_saveAVG = os.path.dirname(
            full_path)+"/diagramm/kontrollAmplitude_ampl"+m+f+".png"
        df1 = pd.read_csv(
            path1, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        df2 = pd.read_csv(
            path2, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        df4 = pd.read_csv(
            path4, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        df8 = pd.read_csv(
            path8, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        df16 = pd.read_csv(
            path16, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        df32 = pd.read_csv(
            path32, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        df64 = pd.read_csv(
            path64, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        
        plt.plot(df1["n"]*df1["n"], df1["AVG["+m+f+"/"+m+"]"],
                 marker='o', label="a=1")
        plt.plot(df2["n"]*df2["n"], df2["AVG["+m+f+"/"+m+"]"],
                 marker='o', label="a=2")
        plt.plot(df4["n"]*df4["n"], df4["AVG["+m+f+"/"+m+"]"],
                 marker='o', label="a=4")
        plt.plot(df8["n"]*df8["n"], df8["AVG["+m+f+"/"+m+"]"],
                 marker='o', label="a=8")
        plt.plot(df16["n"]*df16["n"], df16["AVG["+m+f+"/"+m+"]"],
                 marker='o', label="a=16")
        plt.plot(df32["n"]*df32["n"], df32["AVG["+m+f+"/"+m+"]"],
            marker='o', label="a=32")
        plt.plot(df64["n"]*df64["n"], df64["AVG["+m+f+"/"+m+"]"],
            marker='o', label="a=64")

        plt.legend(loc='upper left')

        plt.xlabel("Knotenanzahl V")
        plt.title("Lösungsqualität AVG "+m+f+"/"+m)
        plt.savefig(path_saveAVG, format='png')
        plt.cla()

M = ["SL", "CL"]
F = ["DN", "FDN"]
for m in M:
    for f in F:
        path1 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude1.txt"
        path2 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude2.txt"
        path4 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude4.txt"
        path8 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude8.txt"
        path16 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude16.txt"
        path32 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude32.txt"
        path64 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/kontrollAmplitude64.txt"
        path_saveAVG = os.path.dirname(
            full_path)+"/diagramm/kontrollAmplitude_ampl"+m+"_FDN.png"
        df1 = pd.read_csv(
            path1, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        df2 = pd.read_csv(
            path2, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        df4 = pd.read_csv(
            path4, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        df8 = pd.read_csv(
            path8, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        df16 = pd.read_csv(
            path16, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        df32 = pd.read_csv(
            path32, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])
        df64 = pd.read_csv(
            path64, sep=",", skiprows=4, names=["n", "a",  "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]"])

        plt.plot(df1["n"]*df1["n"], df1["AVG["+m+"DN/"+m+"FDN]"],
                 marker='o', label="a=1")
        plt.plot(df2["n"]*df2["n"], df2["AVG["+m+"DN/"+m+"FDN]"],
                 marker='o', label="a=2")
        plt.plot(df4["n"]*df4["n"], df4["AVG["+m+"DN/"+m+"FDN]"],
                 marker='o', label="a=4")
        plt.plot(df8["n"]*df8["n"], df8["AVG["+m+"DN/"+m+"FDN]"],
                 marker='o', label="a=8")
        plt.plot(df16["n"]*df16["n"], df16["AVG["+m+"DN/"+m+"FDN]"],
                 marker='o', label="a=16")
        plt.plot(df32["n"]*df32["n"], df32["AVG["+m+"DN/"+m+"FDN]"],
                 marker='o', label="a=32")
        plt.plot(df64["n"]*df64["n"], df64["AVG["+m+"DN/"+m+"FDN]"],
                 marker='o', label="a=64")
        plt.legend(loc='upper left')

        plt.xlabel("Knotenanzahl V")
        plt.title("Lösungsqualität AVG "+m+"DN/"+m+"FDN")
        plt.savefig(path_saveAVG, format='png')
        plt.cla()


M = ["SL", "CL"]
F = ["DN", "FDN"]
#Hier kann man weitere Generatoren hinzuefuegen
ZT = ["APPROX_GT_type2a", "APPROX_GT_type2b", "APPROX_GT_type2c","APPROX_GT_type3"]

for zt in ZT:
    for m in M:
        for f in F:
            path1 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/"+zt+"1GT.txt"
            path2 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/"+zt+"2GT.txt"
            path3 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/"+zt+"3GT.txt"
            path4 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/"+zt+"4GT.txt"
            path5 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/"+zt+"5GT.txt"
            path6 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/"+zt+"6GT.txt"
            path_saveAVG = os.path.dirname(
                full_path)+"/diagramm/GT"+zt+m+"_FDN.png"
            df1 = pd.read_csv(
                path1, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])
            df2 = pd.read_csv(
                path2, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])
            df3 = pd.read_csv(
                path3, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])
            df4 = pd.read_csv(
                path4, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])
            df5 = pd.read_csv(
                path5, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])
            df6 = pd.read_csv(
                path6, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])

            plt.plot(df1["n"]*df1["n"], df1["AVG["+m+"DN/"+m+"FDN]"],
                     marker='o', label="error=1")
            plt.plot(df2["n"]*df2["n"], df2["AVG["+m+"DN/"+m+"FDN]"],
                     marker='o', label="error=2")
            plt.plot(df3["n"]*df3["n"], df3["AVG["+m+"DN/"+m+"FDN]"],
                     marker='o', label="error=3")
            plt.plot(df4["n"]*df4["n"], df4["AVG["+m+"DN/"+m+"FDN]"],
                     marker='o', label="error=4")
            plt.plot(df5["n"]*df5["n"], df5["AVG["+m+"DN/"+m+"FDN]"],
                     marker='o', label="error=5")
            plt.plot(df6["n"]*df6["n"], df6["AVG["+m+"DN/"+m+"FDN]"],
                     marker='o', label="error=6")
            plt.legend(loc='upper left')

            plt.xlabel("Knotenanzahl V")
            plt.title(zt+"\nLösungsqualität AVG "+m+"DN/"+m+"FDN")
            plt.savefig(path_saveAVG, format='png')
            plt.cla()

    for m in M:
        for f in F:
            path1 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/"+zt+"1GT.txt"
            path2 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/"+zt+"2GT.txt"
            path3 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/"+zt+"3GT.txt"
            path4 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/"+zt+"4GT.txt"
            path5 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/"+zt+"5GT.txt"
            path6 = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/"+zt+"6GT.txt"
            path_saveAVG = os.path.dirname(
                full_path)+"/diagramm/GT"+zt+m+f+".png"
            df1 = pd.read_csv(
                path1, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])
            df2 = pd.read_csv(
                path2, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])
            df3 = pd.read_csv(
                path3, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])
            df4 = pd.read_csv(
                path4, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])
            df5 = pd.read_csv(
                path5, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])
            df6 = pd.read_csv(
                path6, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])

            plt.plot(df1["n"]*df1["n"], df1["AVG["+m+f+"/"+m+"]"],
                     marker='o', label="error=1")
            plt.plot(df2["n"]*df2["n"], df2["AVG["+m+f+"/"+m+"]"],
                     marker='o', label="error=2")
            plt.plot(df3["n"]*df3["n"], df3["AVG["+m+f+"/"+m+"]"],
                     marker='o', label="error=3")
            plt.plot(df4["n"]*df4["n"], df4["AVG["+m+f+"/"+m+"]"],
                     marker='o', label="error=4")
            plt.plot(df5["n"]*df5["n"], df5["AVG["+m+f+"/"+m+"]"],
                     marker='o', label="error=5")
            plt.plot(df6["n"]*df6["n"], df6["AVG["+m+f+"/"+m+"]"],
                     marker='o', label="error=6")

            plt.legend(loc='upper left')

            plt.xlabel("Knotenanzahl V")
            plt.title(zt+"\nLösungsqualität AVG "+m+f+"/"+m)

            plt.savefig(path_saveAVG, format='png')
            plt.cla()

    TYPE = [1, 2, 3, 4, 5, 6]
    for type in TYPE:
        path = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/" + \
            zt+str(type)+"GT.txt"
        path_saveAVG = os.path.dirname(
            full_path)+"/diagramm/GT" + zt + str(type)+".png"
        df = pd.read_csv(
            path, sep=",", skiprows=4, names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])
        print(df)
        plt.plot(df["n"]*df["n"], df["AVG[SLFDN/SL]"],
                 marker='o', label="AVG SLFDN/SL")
        plt.plot(df["n"]*df["n"], df["AVG[SLDN/SL]"],
                 marker='o', label="AVG SLDN/SL")
        plt.plot(df["n"]*df["n"], df["AVG[CLFDN/CL]"],
                 marker='o', label="AVG CLFDN/CL")
        plt.plot(df["n"]*df["n"], df["AVG[CLDN/CL]"],
                 marker='o', label="AVG CLDN/CL")

        plt.legend(loc='upper left')
        plt.xlabel("Knotenanzahl V")
        plt.title(zt+"\nDurchschnittliche Lösungsqualität error="+str(type))
        plt.savefig(path_saveAVG, format='png')
        plt.cla()


for zt in ZT:
    for m in M:
        path = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/" + \
            zt+"nicht_aggregiert.txt"

        path_saveAVG = os.path.dirname(
            full_path)+"/diagramm/" + zt+m+"qualityNichtAgg.png"
        df = pd.read_csv(
            path, skiprows=4, sep=",", names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])

        plt.scatter(df["n"], df["AVG["+m+"FDN]"],
                    marker='x', label=m+"FDN")
        plt.scatter(df["n"], df["AVG["+m+"DN]"],
                    marker='.', label=m+"DN")

        plt.legend(loc='upper left')

        plt.xlabel("Knotenanzahl V")
        plt.title("Lösungsqualität")
        plt.savefig(path_saveAVG, format='png')
        plt.cla()


for zt in ZT:
    path = os.getcwd()+"/zeitreihenClustering/results/LSGquality/results/" + \
        zt+"nicht_aggregiert.txt"

    df = pd.read_csv(
        path, skiprows=4, sep=",", names=["n", "AVG[SL]", "AVG[SLFDN]", "AVG[SLDN]", "AVG[SLFDN/SL]", "AVG[SLDN/SL]", "AVG[SLDN/SLFDN]", "AVG[CL]", "AVG[CLFDN]", "AVG[CLDN]", "AVG[CLFDN/CL]", "AVG[CLDN/CL]", "AVG[CLDN/CLFDN]"])
    path_saveAVG = os.path.dirname(
        full_path)+"/diagramm/" + zt+"qualityNichtAggFDN.png"

    plt.scatter(df["n"], df["AVG["+m+"DN/"+m+"FDN]"],
                marker='x', label=m+"DN/"+m+"FDN")

    plt.title("Lösungsqualität")
    plt.legend(loc='upper left')
    plt.xticks([4, 8, 12, 16, 20])
    plt.savefig(path_saveAVG, format='png')
    plt.cla()
