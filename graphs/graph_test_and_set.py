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

files_csv=["mesures_test_and_set.csv", "mesures_test_and_test_and_set.csv"]

data_tas = pd.read_csv(files_csv[0])
data_TTAS = pd.read_csv(files_csv[1])

ecart_tas=pd.read_csv(files_csv[0], usecols=['sec_1', 'sec_2', 'sec_3', 'sec_4', 'sec_5'])
ecart_TTAS=pd.read_csv(files_csv[1], usecols=['sec_1', 'sec_2', 'sec_3', 'sec_4', 'sec_5'])


plt.figure(figsize=(14.8, 12.2))
plt.title('measuresements of time for TAS and TTAS')

x = range(len(data_tas['coeur']))
plt.errorbar(x, data_tas['moyenne'], get_std(ecart_tas), capsize=10)
plt.xticks(x, data_tas['coeur'])

x = range(len(data_TTAS['coeur']))
plt.errorbar(x, data_TTAS['moyenne'], get_std(ecart_TTAS), capsize=10)
plt.xticks(x, data_TTAS['coeur'])

plt.xlabel('number of threads')
plt.ylabel('average time to execute (seconds)')
plt.ylim(ymin=0)
plt.legend(["test-and-set", "test-and-test-and-set"])
plt.savefig('graphs/graphs_png/graph_test_test_and_set.png')

plt.figure().clear()