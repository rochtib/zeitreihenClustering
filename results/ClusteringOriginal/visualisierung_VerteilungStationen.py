print("Visualisier der Ergebnisdatei des Clustering")

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sns
import os
import matplotlib.pyplot as plt
from matplotlib import colors as mcolors

full_path = os.path.realpath(__file__)
TypeAlgo = ['SL','SLDN','SLFDN','CL','CLDN','CLFDN']

for type in TypeAlgo:
    path=os.getcwd()+"/zeitreihenClustering/results/ClusteringOriginal/Clustering_"+type+".txt"
    path_save=os.path.dirname(full_path) + "/Clustering_"+type+".png"
    df = pd.read_csv(
        path, sep=" ",names=["x","y","c","d"])
    print(df)

    x_koordinate = df['x']
    y_koordinate = df['y']
    cluster = df['c']
    station_name = df['d']
    fig, ax = plt.subplots()
    ax.scatter(x_koordinate, y_koordinate) #color=colors[df['c']]


    for i, txt in enumerate(station_name):
        txt ="[" + str(cluster[i]) +"]" + station_name[i]
        ax.annotate(txt, (x_koordinate[i], y_koordinate[i]),xytext=(x_koordinate[i],y_koordinate[i]+0.4), ha='center')
    plt.xlabel("X - Koordinate")
    plt.ylabel("Y - Koordinate")
    title_cluster="Clustering "+type+" von Meeresspiegelmessungen"
    plt.title(title_cluster,fontsize=15)
    plt.savefig(path_save, format='png')
    #plt.show()  

