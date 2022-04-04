import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman'] # 
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = ["0.3M","1M","3M","5M","7M","9M"]

Rein = [2.713345, 8.232349, 21.414993, 37.706974, 55.908721, 69.915062]
HEM = [0.195233, 0.618994, 1.962566, 3.380389, 4.932515, 12.299295]
# Simple = [4.038256, 13.878217, 40.736844, 68.019431, 94.269526, 121.051676]
TAMA = [0.704289, 3.219745, 13.40329, 26.586104, 50.579369, 81.963197]
AdaREIN = [2.720448, 8.021952, 20.41238, 36.483386, 54.157477, 72.381039]
OpIndex = [5.371004, 18.298927, 55.104239, 91.46666, 132.142078, 182.600677]

lsize=24

fig=plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Number of Subscriptions', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend(fontsize=12, ncol=2,loc='lower right') #fontsize=10 loc=(1.36/5,0.05/5),
ax.grid()
ax.set_xlim(0,5)
ax.set_xticks([0,1,2,3,4,5])
ax.set_xticklabels(x)
ax.set_yscale("log")
# ax.set_yticks([0,2,8,32,128,256])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=22)
gcf = plt.gcf()
plt.show()
gcf.savefig('../exp3_n.eps',format='eps',bbox_inches='tight')