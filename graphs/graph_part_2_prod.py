#!/usr/bin/env python3
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def get_std(array):
    arr = [0] * 8
    i=0

    while i<8:
        arr[i]=np.std(array.T[i])
        i=i+1

    return arr

files_csv=["mesures_prod.csv", "mesures_prod_TAS.csv", "mesures_prod_ttas.csv"]

data_prod_normal = pd.read_csv(files_csv[0])
data_prod_TAS = pd.read_csv(files_csv[1])
data_prod_ttas = pd.read_csv(files_csv[2])

ecart_prod_normal=pd.read_csv(files_csv[0], usecols=['sec_1', 'sec_2', 'sec_3', 'sec_4', 'sec_5'])
ecart_prod_TAS=pd.read_csv(files_csv[1], usecols=['sec_1', 'sec_2', 'sec_3', 'sec_4', 'sec_5'])
ecart_prod_ttas=pd.read_csv(files_csv[2], usecols=['sec_1', 'sec_2', 'sec_3', 'sec_4', 'sec_5'])


plt.figure(figsize=(14.8, 12.2))
plt.title('measuresements of time through the prod algorithm')

x = range(len(data_prod_normal['thread']))
"""plt.plot(x, data_prod_normal['moyenne'])"""
plt.errorbar(x, data_prod_normal['moyenne'], get_std(ecart_prod_normal), capsize=10)
plt.xticks(x, data_prod_normal['thread'])


x = range(len(data_prod_TAS['thread']))
"""plt.plot(x, data_prod_TAS['moyenne'])"""
plt.errorbar(x, data_prod_TAS['moyenne'], get_std(ecart_prod_TAS), capsize=10)
plt.xticks(x, data_prod_TAS['thread'])

x = range(len(data_prod_ttas['thread']))
"""plt.plot(x, data_prod_ttas['moyenne'])"""
plt.errorbar(x, data_prod_ttas['moyenne'], get_std(ecart_prod_ttas), capsize=10)
plt.xticks(x, data_prod_ttas['thread'])

plt.xlabel('number of threads')
plt.ylabel('average time to execute')
plt.ylim(ymin=0)
plt.legend(["prod", "prod TAS", "prod ttas"])
plt.savefig('graphs/graphs_png/graph_prod.png')

plt.show()