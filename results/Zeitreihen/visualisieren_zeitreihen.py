import matplotlib.pyplot as plt
import pandas as pd
import os

import unicodedata

full_path = os.path.realpath(__file__)

TYPE = []  #

for type in TYPE:
    Zeit = list(range(1, 41))
    path = os.getcwd()+"/zeitreihenClustering/results/Zeitreihen/Daten/"+type+"_1.txt"
    df = pd.read_csv(
        path, sep=",", names=["z"])
    print(df)
    plt.plot(Zeit, df["z"], label="Approx1")

    path = os.getcwd()+"/zeitreihenClustering/results/Zeitreihen/Daten/"+type+"_2.txt"
    df = pd.read_csv(
        path, sep=",", names=["z"])
    print(df)
    plt.plot(Zeit, df["z"], label="Approx2")

    path = os.getcwd()+"/zeitreihenClustering/results/Zeitreihen/Daten/"+type+"_3.txt"
    df = pd.read_csv(
        path, sep=",", names=["z"])
    print(df)
    plt.plot(Zeit, df["z"], label="Approx3")

    ax = plt.gca()
    ax.axes.xaxis.set_visible(False)
    ax.axes.yaxis.set_visible(False)

    path_saveAVG = os.path.dirname(full_path) + "/diagramm/"+type+".png"
    plt.savefig(path_saveAVG, format='png')
    # plt.show()

    plt.cla()

TYPE = ["Real"]

for type in TYPE:
    Zeit = list(range(1, 41))
    path = os.getcwd()+"/zeitreihenClustering/results/Zeitreihen/Daten/"+type+"_1.txt"
    df = pd.read_csv(
        path, sep=";", names=["jahr", "z", "dummy2", "dummy3"])
    print(df)
    plt.plot(Zeit, df["z"], label="Cuxhaven")  # 7 -> hier nummer 1 monthly

    path = os.getcwd()+"/zeitreihenClustering/results/Zeitreihen/Daten/"+type+"_2.txt"
    df = pd.read_csv(
        path, sep=";", names=["jahr", "z", "dummy2", "dummy3"])
    print(df)
    plt.plot(Zeit, df["z"], label="Travemünde")  # 13 -> hier nummer 2 monthly

    path = os.getcwd()+"/zeitreihenClustering/results/Zeitreihen/Daten/"+type+"_3.txt"
    df = pd.read_csv(
        path, sep=";", names=["jahr", "z", "dummy2", "dummy3"])
    print(df)
    plt.plot(Zeit, df["z"], label="Liverpool")  # 1774 -> hier nummer 3 monthly

    path = os.getcwd()+"/zeitreihenClustering/results/Zeitreihen/Daten/"+type+"_4.txt"
    df = pd.read_csv(
        path, sep=";", names=["jahr", "z", "dummy2", "dummy3"])
    print(df)
    plt.plot(Zeit, df["z"], label="Sydney")  # 2358 -> hier nummer 4 monthly
    plt.legend(loc='upper center')

    ax = plt.gca()
    ax.axes.xaxis.set_visible(False)
    ax.axes.yaxis.set_visible(False)

    path_saveAVG = os.path.dirname(full_path)+"/diagramm/"+type+".png"
    plt.savefig(path_saveAVG, format='png')
    # plt.show()

    plt.cla()


# Label für die y-Achse vergeben:
plt.ylabel('Quadratzahlen')

# Einen x-y-Plot erstellen:
pix = [1, 3, 6, 9, 12, 15]
piy = [1, 10, 1, 10, 1, 10]
plt.plot(pix, piy, linewidth=3, color='black', marker='o',markersize=8,
         label=unicodedata.lookup("GREEK SMALL LETTER PI"))

pix = [1, 15, 1, 15, 1, 15]
piy = [9, 7, 6, 5, 4, 3]
plt.plot(pix, piy, linewidth=3, color='red', marker='o', markersize=8,
         label=unicodedata.lookup("GREEK SMALL LETTER SIGMA"))

plt.legend()

# Ein gepunktetes Diagramm-Gitter einblenden:
plt.grid(True)
path_saveAVG = os.path.dirname(full_path)+"/diagramm/hausdorffrechet.png"
plt.savefig(path_saveAVG, format='png')
