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

files_csv=["mesures_reader.csv", "mesures_reader_TAS.csv", "mesures_reader_ttas.csv"]

data_normal = pd.read_csv(files_csv[0])
data_TAS = pd.read_csv(files_csv[1])
data_ttas = pd.read_csv(files_csv[2])

ecart_normal=pd.read_csv(files_csv[0], usecols=['sec_1', 'sec_2', 'sec_3', 'sec_4', 'sec_5'])
ecart_TAS=pd.read_csv(files_csv[1], usecols=['sec_1', 'sec_2', 'sec_3', 'sec_4', 'sec_5'])
ecart_ttas=pd.read_csv(files_csv[2], usecols=['sec_1', 'sec_2', 'sec_3', 'sec_4', 'sec_5'])


plt.figure(figsize=(14.8, 12.2))
plt.title('measuresements of time through the reader algorithm')

x = range(len(data_normal['thread']))
plt.plot(x, data_normal['moyenne'])
plt.errorbar(x, data_normal['moyenne'], get_std(ecart_normal), capsize=10)
plt.xticks(x, data_normal['thread'])

x = range(len(data_TAS['thread']))
plt.plot(x, data_TAS['moyenne'])
plt.errorbar(x, data_TAS['moyenne'], get_std(ecart_TAS), capsize=10)
plt.xticks(x, data_TAS['thread'])

x = range(len(data_ttas['thread']))
plt.plot(x, data_ttas['moyenne'])
plt.errorbar(x, data_ttas['moyenne'], get_std(ecart_ttas), capsize=10)
plt.xticks(x, data_ttas['thread'])

plt.xlabel('number of threads')
plt.ylabel('average time to execute')
plt.ylim(ymin=0)
plt.legend(["reader", "reader TAS", "reader ttas"])
plt.savefig('graphs/graph_reader_ttas.png')