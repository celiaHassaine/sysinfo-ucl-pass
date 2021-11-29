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
data_philo = pd.read_csv('mesures_philo.csv')
data_reader = pd.read_csv('mesures_reader.csv')
plt.figure(figsize=(14.8, 12.2))
plt.title('measuresements of time through 3 different algorithm')

x = range(len(data_prod['coeur']))
plt.plot(x, data_prod['moyenne'])
plt.xticks(x, data_prod['coeur'])

x = range(len(data_philo['coeur']))
plt.plot(x, data_philo['moyenne'])
plt.xticks(x, data_philo['coeur'])

x = range(len(data_reader['coeur']))
plt.plot(x, data_reader['moyenne'])
plt.xticks(x, data_reader['coeur'])

plt.xlabel('number of threads')
plt.ylabel('average time to execute')
plt.legend(["prod-cons algo", "philo algo", "reader algo"])
plt.show()