#!/usr/bin/env python3
from os import sep
import matplotlib.pyplot as plt
from numpy.core.fromnumeric import prod
import pandas as pd
import numpy as np
import os

prog_names=["prod", "philo", "reader"]

i = 0
data_prod = pd.read_csv('mesures_prod.csv')
#data_philo = pd.read_csv('mesures_philo.csv')
#data_reader = pd.read_csv('mesures_reader.csv')
plt.figure(figsize=(14.8, 12.2))
plt.title('measuresements of time through 3 different algorithm')

x = range(len(data_prod['coeur']))
plt.plot(x, data_prod['moyenne'])
plt.xticks(x, data_prod['coeur'])
"""
x = range(len(data_philo['coeur']))
plt.plot(x, data_philo['moyenne'])
plt.xticks(x, data_philo['coeur'])

x = range(len(data_reader['coeur']))
plt.plot(x, data_reader['moyenne'])
plt.xticks(x, data_reader['coeur'])
"""
plt.xlabel('number of threads')
plt.ylabel('average time to execute')
plt.legend(["prod-cons algo", "philo algo", "reader algo"])
plt.show()
"""
# Load data
file = 'mesures_'+prog_names[i]+'.csv'
print(file)
if os.path.isfile(file):
    print("here")
    data = pd.read_csv(file)
# Plot
#plt.figure(figsize=(6.8, 4.2))
if prog_names[i] == "prod":
    plt.title('measures from prod-cons algorithm')
elif prog_names[i] == "philo":
    plt.title('measures from philo algorithm')
elif prog_names[i] == "reader":
    plt.title('measures from reader algorithm')
x = range(len(data['coeur']))
plt.plot(x, data['moyenne'])
plt.xticks(x, data['coeur'])
plt.xlabel('number of threads')
plt.ylabel('average time to execute')
plt.show()

i=i+1"""