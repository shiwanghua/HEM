import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman']  #
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = ["0.3M", "1M", "3M", "5M", "7M", "9M"]

REIN = [46, 153, 458, 763, 1069, 1374]
HEM5DD = [69, 227, 680, 1133, 1586, 2039]
HEM5_VAS = [69, 228, 681, 1134, 1587, 2040]
TAMA = [146, 487, 1461, 2435, 3409, 4384]
AdaREIN = [45, 152, 457, 762, 1068, 1373]
Simple = [36, 122, 366, 610, 854, 1098]
OpIndex = [34, 114, 343, 572, 801, 1029]

lsize = 24

fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Number of Subscriptions', fontsize=lsize)
ax.set_ylabel('Memory Consumption (MB)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, REIN, marker='v', color='r', label=Name[0])
ax.plot(x, HEM5_VAS, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5])  #   slategray

ax.legend(fontsize=14, ncol=1)  #fontsize=10 loc=(1.36/5,0.05/5),
ax.grid()
ax.set_xlim(0, 5)
ax.set_xticks([0, 1, 2, 3, 4, 5])
ax.set_xticklabels(x)
ax.set_yscale("log")
# ax.yaxis.set_ticks([0,100,1000,2000])
# ax.set_yticks([0,100,1000,2000])
# ax.set_yticklabels(['0','100','1000','2000'])
ax.set_zorder(0)
plt.tick_params(labelsize=22)
gcf = plt.gcf()
plt.show()
gcf.savefig('./exp15_memory.pdf', format='pdf', bbox_inches='tight')
