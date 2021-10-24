import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False

Name = ["REIN", "BIOP5DD", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = ["0.3m","1m","3m","5m","7m","9m"]

Rein = [45, 152, 457, 762, 1068, 1373	]
BIOP5DD = [68, 226, 679, 1132, 1585, 2038]
Simple = [36, 122, 366, 610, 854, 1098]
TAMA = [141, 470, 1410, 2350, 3291, 4231]
AdaREIN = [45, 152, 457, 762, 1068, 1373]
OpIndex = [34, 114, 343, 572, 801, 1029]			

fig=plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('Number of Subscriptions', fontsize=13)
ax.set_ylabel('Memory Consumption (MB)', fontsize=13)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, BIOP5DD, marker='.', color='DEEPPINK', label=Name[1])
ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend( ncol=3) #fontsize=10 loc=(1.36/5,0.05/5),
ax.grid()
ax.set_xlim(0,5)
ax.set_xticks([0,1,2,3,4,5])
ax.set_xticklabels(x)
ax.set_yscale("log")
# ax.yaxis.set_ticks([0,100,1000,2000])
# ax.set_yticks([0,100,1000,2000])
# ax.set_yticklabels(['0','100','1000','2000'])
ax.set_zorder(0)

gcf = plt.gcf()
plt.show()
gcf.savefig('../exp11_memory.eps',format='eps',bbox_inches='tight')