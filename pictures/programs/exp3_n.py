import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False

Name = ["REIN", "BIOP5DD", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = ["0.3m","1m","3m","5m","7m","9m"]

Rein = [2.747558, 7.338444, 18.947097, 36.257594, 51.005946, 68.03113]
BIOP5DD = [0.379034, 0.983106, 2.93406, 5.030511, 7.074338, 14.886429]
Simple = [4.038256, 13.878217, 40.736844, 68.019431, 94.269526, 121.051676]
TAMA = [0.995051, 4.04467, 13.40329, 26.586104, 50.579369, 81.963197]
AdaREIN = [2.140908, 6.28658, 16.758969, 30.40834, 43.844497, 57.686101]
OpIndex = [6.805552, 23.133089, 68.743382, 117.347896, 173.80194, 243.786402]

fig=plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('Number of Subscriptions', fontsize=13)
ax.set_ylabel('Matching Time (ms)', fontsize=13)
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
# ax.set_yticks([0,2,8,32,128,256])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)

gcf = plt.gcf()
plt.show()
gcf.savefig('../exp3_n.eps',format='eps',bbox_inches='tight')