import matplotlib.pyplot as plt
import pandas as pd
import os
full_path = os.path.realpath(__file__)
#print(os.getcwd())

TYPE = ["FDN", "DN"]
Linkage = ["CL", "SL"]
for type in Linkage:
    for modus in TYPE:
        path_saveVER = os.path.dirname(
            full_path) + "/diagramm/"+type+modus+".png"

        path50 =  os.getcwd()+"/zeitreihenClustering/results/RealWorldData/results/rlr_monthly_N=50_ZR100.txt"
        path100 = os.getcwd()+"/zeitreihenClustering/results/RealWorldData/results/rlr_monthly_N=100_ZR100.txt"
        path200 = os.getcwd()+"/zeitreihenClustering/results/RealWorldData/results/rlr_monthly_N=200_ZR100.txt"
        path300 = os.getcwd()+"/zeitreihenClustering/results/RealWorldData/results/rlr_monthly_N=300_ZR100.txt"
        path400 = os.getcwd()+"/zeitreihenClustering/results/RealWorldData/results/rlr_monthly_N=400_ZR100.txt"
        path500 = os.getcwd()+"/zeitreihenClustering/results/RealWorldData/results/rlr_monthly_N=500_ZR100.txt"

        df50 = pd.read_csv(
            path50, skiprows=6, sep=",", names=["n", "SL", "SLFDN", "[SLFDN/SL]", "SLDN", "[SLDN/SL]","[SLDN/SLFDN]", "CL", "CLFDN", "[CLFDN/CL]", "CLDN", "[CLDN/CL]","[CLDN/CLFDN]"])
        df100 = pd.read_csv(
            path100, skiprows=6, sep=",", names=["n", "SL", "SLFDN", "[SLFDN/SL]", "SLDN", "[SLDN/SL]","[SLDN/SLFDN]", "CL", "CLFDN", "[CLFDN/CL]", "CLDN", "[CLDN/CL]","[CLDN/CLFDN]"])
        df200 = pd.read_csv(
            path200, skiprows=6, sep=",", names=["n", "SL", "SLFDN", "[SLFDN/SL]", "SLDN", "[SLDN/SL]","[SLDN/SLFDN]", "CL", "CLFDN", "[CLFDN/CL]", "CLDN", "[CLDN/CL]","[CLDN/CLFDN]"])
        df300 = pd.read_csv(
            path300, skiprows=6, sep=",", names=["n", "SL", "SLFDN", "[SLFDN/SL]", "SLDN", "[SLDN/SL]","[SLDN/SLFDN]", "CL", "CLFDN", "[CLFDN/CL]", "CLDN", "[CLDN/CL]","[CLDN/CLFDN]"])
        df400 = pd.read_csv(
            path400, skiprows=6, sep=",", names=["n", "SL", "SLFDN", "[SLFDN/SL]", "SLDN", "[SLDN/SL]","[SLDN/SLFDN]", "CL", "CLFDN", "[CLFDN/CL]", "CLDN", "[CLDN/CL]","[CLDN/CLFDN]"])
        df500 = pd.read_csv(
            path500, skiprows=6, sep=",", names=["n", "SL", "SLFDN", "[SLFDN/SL]", "SLDN", "[SLDN/SL]","[SLDN/SLFDN]", "CL", "CLFDN", "[CLFDN/CL]", "CLDN", "[CLDN/CL]","[CLDN/CLFDN]"])
        spalte = "["+type+modus+"/"+type+"]"

        perm_x = [df50.at[0, "n"],   df100.at[0, "n"],   df200.at[0, "n"],
                  df300.at[0, "n"],   df400.at[0, "n"], df500.at[0, "n"]]
        perm_y = [df50.at[0, spalte], df100.at[0, spalte], df200.at[0, spalte],
                  df300.at[0, spalte], df400.at[0, spalte], df500.at[0, spalte]]
        plt.plot(perm_x, perm_y, marker='o',
                 label="Real World "+type+modus+"/"+type)
        orig_x = [df50.at[1, "n"],   df100.at[1, "n"],   df200.at[1, "n"],
                  df300.at[1, "n"],   df400.at[1, "n"], df500.at[1, "n"]]
        orig_y = [df50.at[1, spalte], df100.at[1, spalte], df200.at[1, spalte],
                  df300.at[1, spalte], df400.at[1, spalte], df500.at[1, spalte]]
        plt.plot(orig_x, orig_y, marker='o',
                 label="PERM AVG["+type+modus + "/"+type+"]")
        plt.legend(loc='upper left')
        plt.xlabel("Knotenanzahl V")
        plt.title("Real World PSMSL Daten "+type+modus)
        plt.savefig(path_saveVER, format='png')
        plt.cla()

path_saveVER = os.path.dirname(full_path) + "/diagramm/RealWorld_DN_FDN.png"

path50 = os.getcwd()+"/zeitreihenClustering/results/RealWorldData/results/rlr_monthly_N=50_ZR100.txt"
path100 = os.getcwd()+"/zeitreihenClustering/results/RealWorldData/results/rlr_monthly_N=100_ZR100.txt"
path200 = os.getcwd()+"/zeitreihenClustering/results/RealWorldData/results/rlr_monthly_N=200_ZR100.txt"
path300 = os.getcwd()+"/zeitreihenClustering/results/RealWorldData/results/rlr_monthly_N=300_ZR100.txt"
path400 = os.getcwd()+"/zeitreihenClustering/results/RealWorldData/results/rlr_monthly_N=400_ZR100.txt"
path500 = os.getcwd()+"/zeitreihenClustering/results/RealWorldData/results/rlr_monthly_N=500_ZR100.txt"

df50 = pd.read_csv(
    path50, skiprows=6, sep=",", names=["n", "SL", "SLFDN", "[SLFDN/SL]", "SLDN", "[SLDN/SL]","[SLDN/SLFDN]", "CL", "CLFDN", "[CLFDN/CL]", "CLDN", "[CLDN/CL]","[CLDN/CLFDN]"])
df100 = pd.read_csv(
    path100, skiprows=6, sep=",", names=["n", "SL", "SLFDN", "[SLFDN/SL]", "SLDN", "[SLDN/SL]","[SLDN/SLFDN]", "CL", "CLFDN", "[CLFDN/CL]", "CLDN", "[CLDN/CL]","[CLDN/CLFDN]"])
df200 = pd.read_csv(
    path200, skiprows=6, sep=",", names=["n", "SL", "SLFDN", "[SLFDN/SL]", "SLDN", "[SLDN/SL]","[SLDN/SLFDN]", "CL", "CLFDN", "[CLFDN/CL]", "CLDN", "[CLDN/CL]","[CLDN/CLFDN]"])
df300 = pd.read_csv(
    path300, skiprows=6, sep=",", names=["n", "SL", "SLFDN", "[SLFDN/SL]", "SLDN", "[SLDN/SL]","[SLDN/SLFDN]", "CL", "CLFDN", "[CLFDN/CL]", "CLDN", "[CLDN/CL]","[CLDN/CLFDN]"])
df400 = pd.read_csv(
    path400, skiprows=6, sep=",", names=["n", "SL", "SLFDN", "[SLFDN/SL]", "SLDN", "[SLDN/SL]","[SLDN/SLFDN]", "CL", "CLFDN", "[CLFDN/CL]", "CLDN", "[CLDN/CL]","[CLDN/CLFDN]"])
df500 = pd.read_csv(
    path500, skiprows=6, sep=",", names=["n", "SL", "SLFDN", "[SLFDN/SL]", "SLDN", "[SLDN/SL]","[SLDN/SLFDN]", "CL", "CLFDN", "[CLFDN/CL]", "CLDN", "[CLDN/CL]","[CLDN/CLFDN]"])

orig_x = [df50.at[0, "n"],   df100.at[0, "n"],   df200.at[0, "n"],
          df300.at[0, "n"],   df400.at[0, "n"], df500.at[0, "n"]]
orig_y = [df50.at[0, "[SLDN/SLFDN]"], df100.at[0, "[SLDN/SLFDN]"], df200.at[0, "[SLDN/SLFDN]"],
          df300.at[0, "[SLDN/SLFDN]"], df400.at[0, "[SLDN/SLFDN]"], df500.at[0, "[SLDN/SLFDN]"]]
plt.plot(orig_x, orig_y, marker='o', label="SLDN/SLFDN")

orig_x = [df50.at[0, "n"],   df100.at[0, "n"],   df200.at[0, "n"],
          df300.at[0, "n"],   df400.at[0, "n"], df500.at[0, "n"]]
orig_y = [df50.at[0, "[CLDN/CLFDN]"], df100.at[0, "[CLDN/CLFDN]"], df200.at[0, "[CLDN/CLFDN]"],
          df300.at[0, "[CLDN/CLFDN]"], df400.at[0, "[CLDN/CLFDN]"], df500.at[0, "[CLDN/CLFDN]"]]
plt.plot(orig_x, orig_y, marker='o', label="CLDN/CLFDN")

orig_x = [df50.at[1, "n"],   df100.at[1, "n"],   df200.at[1, "n"],
          df300.at[1, "n"],   df400.at[1, "n"], df500.at[1, "n"]]
orig_y = [df50.at[1, "[SLDN/SLFDN]"], df100.at[1, "[SLDN/SLFDN]"], df200.at[1, "[SLDN/SLFDN]"],
          df300.at[1, "[SLDN/SLFDN]"], df400.at[1, "[SLDN/SLFDN]"], df500.at[1, "[SLDN/SLFDN]"]]
plt.plot(orig_x, orig_y, marker='o', label="PERM AVG[SLDN/SLFDN]")

orig_x = [df50.at[1, "n"],   df100.at[1, "n"],   df200.at[1, "n"],
          df300.at[1, "n"],   df400.at[1, "n"], df500.at[1, "n"]]
orig_y = [df50.at[1, "[CLDN/CLFDN]"], df100.at[1, "[CLDN/CLFDN]"], df200.at[1, "[CLDN/CLFDN]"],
          df300.at[1, "[CLDN/CLFDN]"], df400.at[1, "[CLDN/CLFDN]"], df500.at[1, "[CLDN/CLFDN]"]]
plt.plot(orig_x, orig_y, marker='o', label="PERM AVG[CLDN/CLFDN]")

plt.legend(loc='upper left')
plt.xlabel("Knotenanzahl V")
plt.title("Real World PSMSL DN/FDN")
plt.savefig(path_saveVER, format='png')
plt.cla()
