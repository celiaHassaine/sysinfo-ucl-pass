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

files_csv=["mesures_prod.csv", "mesures_philo.csv", "mesures_reader.csv"]

data_prod = pd.read_csv(files_csv[0])
data_philo = pd.read_csv(files_csv[1])
data_reader = pd.read_csv(files_csv[2])

ecart_prod=pd.read_csv(files_csv[0], usecols=['sec_1', 'sec_2', 'sec_3', 'sec_4', 'sec_5'])
ecart_philo=pd.read_csv(files_csv[1], usecols=['sec_1', 'sec_2', 'sec_3', 'sec_4', 'sec_5'])
ecart_reader=pd.read_csv(files_csv[2], usecols=['sec_1', 'sec_2', 'sec_3', 'sec_4', 'sec_5'])


plt.figure(figsize=(14.8, 12.2))
plt.title('measuresements of time through PROD algorithm')

x = range(len(data_prod['thread']))
plt.plot(x, data_prod['moyenne'])
plt.errorbar(x, data_prod['moyenne'], get_std(ecart_prod), capsize=10, fmt=' ', ecolor='green')
plt.xticks(x, data_prod['thread'])
plt.xlabel('number of threads')
plt.ylabel('average time to execute (seconds)')
plt.ylim(ymin=0)
plt.legend(["prod-cons moyenne", "prod-cons ecart-type"])
plt.savefig('graphs/graphs_png/graph_prod_part1.png')

plt.figure().clear()
"""
plt.figure(figsize=(14.8, 12.2))
plt.title('measuresements of time through 3 different algorithm')

x = range(len(data_philo['thread']))
plt.plot(x, data_philo['moyenne'])
plt.errorbar(x, data_philo['moyenne'], get_std(ecart_philo), capsize=10, fmt=' ', ecolor='red')
plt.xticks(x, data_philo['thread'])
plt.xlabel('number of threads')
plt.ylabel('average time to execute')
plt.ylim(ymin=0)
plt.legend(["philo moyenne", "philo ecart-type"])
plt.savefig('graphs/graphs_png/graph_philo_part1.png')

plt.figure().clear()

plt.figure(figsize=(14.8, 12.2))
plt.title('measuresements of time through 3 different algorithm')

x = range(len(data_reader['thread']))
plt.plot(x, data_reader['moyenne'])
plt.errorbar(x, data_reader['moyenne'], get_std(ecart_reader), capsize=10, fmt=' ', ecolor='blue')
plt.xticks(x, data_reader['thread'])

plt.xlabel('number of threads')
plt.ylabel('average time to execute')
plt.ylim(ymin=0)
plt.legend(["reader moyenne", "reader ecart-type"])
plt.savefig('graphs/graphs_png/graph_reader_part1.png')"""