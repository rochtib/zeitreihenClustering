import matplotlib.pyplot as plt 
import pandas as pd
import os
full_path = os.path.realpath(__file__)

TYPE = ["FDN","DN"]
Linkage=["CL","SL"]
path_saveVER=os.path.dirname(full_path)+"/diagramm/RealWorldAufCL.png"
for type in Linkage:
    for modus in TYPE:
        path50=os.getcwd()+"/zeitreihenClustering/results/SLaufCL/results/rlr_monthly_N=50_ZR100TEST.txt"
        path100=os.getcwd()+"/zeitreihenClustering/results/SLaufCL/results/rlr_monthly_N=100_ZR100TEST.txt"
        path200=os.getcwd()+"/zeitreihenClustering/results/SLaufCL/results/rlr_monthly_N=200_ZR100TEST.txt"
        path300=os.getcwd()+"/zeitreihenClustering/results/SLaufCL/results/rlr_monthly_N=300_ZR100TEST.txt"
        path400=os.getcwd()+"/zeitreihenClustering/results/SLaufCL/results/rlr_monthly_N=400_ZR100TEST.txt"
        path500=os.getcwd()+"/zeitreihenClustering/results/SLaufCL/results/rlr_monthly_N=500_ZR100TEST.txt"

        df50 = pd.read_csv(   
            path50,skiprows=6, sep=",",names=["n","SL","SLFDN","[SLFDN/SL]","SLDN","[SLDN/SL]","CL","CLFDN","[CLFDN/CL]","CLDN","[CLDN/CL]"])
        df100 = pd.read_csv(
            path100,skiprows=6, sep=",",names=["n","SL","SLFDN","[SLFDN/SL]","SLDN","[SLDN/SL]","CL","CLFDN","[CLFDN/CL]","CLDN","[CLDN/CL]"])
        df200 = pd.read_csv(
            path200,skiprows=6, sep=",",names=["n","SL","SLFDN","[SLFDN/SL]","SLDN","[SLDN/SL]","CL","CLFDN","[CLFDN/CL]","CLDN","[CLDN/CL]"])
        df300 = pd.read_csv(
            path300,skiprows=6, sep=",",names=["n","SL","SLFDN","[SLFDN/SL]","SLDN","[SLDN/SL]","CL","CLFDN","[CLFDN/CL]","CLDN","[CLDN/CL]"])
        df400 = pd.read_csv(
            path400,skiprows=6, sep=",",names=["n","SL","SLFDN","[SLFDN/SL]","SLDN","[SLDN/SL]","CL","CLFDN","[CLFDN/CL]","CLDN","[CLDN/CL]"])
        df500 = pd.read_csv(
            path500,skiprows=6, sep=",",names=["n","SL","SLFDN","[SLFDN/SL]","SLDN","[SLDN/SL]","CL","CLFDN","[CLFDN/CL]","CLDN","[CLDN/CL]"])
        spalte="["+type+modus+"/"+type+"]"

        perm_x=[df50.at[0,"n"],   df100.at[0,"n"],   df200.at[0,"n"],   df300.at[0,"n"],   df400.at[0,"n"],df500.at[0,"n"]]
        perm_y=[df50.at[0,spalte],df100.at[0,spalte],df200.at[0,spalte],df300.at[0,spalte],df400.at[0,spalte],df500.at[0,spalte]]
        plt.plot(perm_x,perm_y, marker='o',label=type+modus+"/CL")
        plt.legend(loc='upper left')
        plt.xlabel("Knotenanzahl V")
        plt.title("Real World PSMSL Daten mit CL ausgewertet")
plt.savefig(path_saveVER, format='png')
plt.cla()

path_saveVER=os.path.dirname(full_path)+"/diagramm/permAufCL.png"
for type in Linkage:
    for modus in TYPE:
        path50=os.getcwd()+"/zeitreihenClustering/results/SLaufCL/results/rlr_monthly_N=50_ZR100TEST.txt"
        path100=os.getcwd()+"/zeitreihenClustering/results/SLaufCL/results/rlr_monthly_N=100_ZR100TEST.txt"
        path200=os.getcwd()+"/zeitreihenClustering/results/SLaufCL/results/rlr_monthly_N=200_ZR100TEST.txt"
        path300=os.getcwd()+"/zeitreihenClustering/results/SLaufCL/results/rlr_monthly_N=300_ZR100TEST.txt"
        path400=os.getcwd()+"/zeitreihenClustering/results/SLaufCL/results/rlr_monthly_N=400_ZR100TEST.txt"
        path500=os.getcwd()+"/zeitreihenClustering/results/SLaufCL/results/rlr_monthly_N=500_ZR100TEST.txt"

        df50 = pd.read_csv(   
            path50,skiprows=6, sep=",",names=["n","SL","SLFDN","[SLFDN/SL]","SLDN","[SLDN/SL]","CL","CLFDN","[CLFDN/CL]","CLDN","[CLDN/CL]"])
        df100 = pd.read_csv(
            path100,skiprows=6, sep=",",names=["n","SL","SLFDN","[SLFDN/SL]","SLDN","[SLDN/SL]","CL","CLFDN","[CLFDN/CL]","CLDN","[CLDN/CL]"])
        df200 = pd.read_csv(
            path200,skiprows=6, sep=",",names=["n","SL","SLFDN","[SLFDN/SL]","SLDN","[SLDN/SL]","CL","CLFDN","[CLFDN/CL]","CLDN","[CLDN/CL]"])
        df300 = pd.read_csv(
            path300,skiprows=6, sep=",",names=["n","SL","SLFDN","[SLFDN/SL]","SLDN","[SLDN/SL]","CL","CLFDN","[CLFDN/CL]","CLDN","[CLDN/CL]"])
        df400 = pd.read_csv(
            path400,skiprows=6, sep=",",names=["n","SL","SLFDN","[SLFDN/SL]","SLDN","[SLDN/SL]","CL","CLFDN","[CLFDN/CL]","CLDN","[CLDN/CL]"])
        df500 = pd.read_csv(
            path500,skiprows=6, sep=",",names=["n","SL","SLFDN","[SLFDN/SL]","SLDN","[SLDN/SL]","CL","CLFDN","[CLFDN/CL]","CLDN","[CLDN/CL]"])
        
        spalte="["+type+modus+"/"+type+"]"

        orig_x=[df50.at[1,"n"],   df100.at[1,"n"],   df200.at[1,"n"],   df300.at[1,"n"],   df400.at[1,"n"],df500.at[1,"n"]]
        orig_y=[df50.at[1,spalte],df100.at[1,spalte],df200.at[1,spalte],df300.at[1,spalte],df400.at[1,spalte],df500.at[1,spalte]]
        plt.plot(orig_x,orig_y, marker='o',label="AVG "+type+modus+"/CL")
        plt.legend(loc='upper left')
        plt.xlabel("Knotenanzahl V")
        plt.title("Permutierte Real World PSMSL Daten auf CL ausgewertet")
plt.savefig(path_saveVER, format='png')
plt.cla()


