import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman']  #
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [30, 40, 50, 60, 70, 80]

Rein = [6.567713, 6.788912, 7.238252, 7.625114, 8.052776, 8.402323]
AdaREIN = [6.717479, 6.778906, 7.234105, 7.626720, 8.045197, 8.424379]
HEM = [1.197435, 1.114701, 1.031818, 0.936328, 0.834270, 0.775228]
TAMA = [3.510166, 3.195298, 2.942713, 2.715438, 2.464011, 2.242568]
OpIndex = [13.452053, 10.852545, 8.605018, 6.728184, 5.096041, 3.685943]

Rein.reverse()
AdaREIN.reverse()
HEM.reverse()
TAMA.reverse()
OpIndex.reverse()

lsize = 24

fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Size of Events', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5])  #   slategray

ax.legend(fontsize=14, ncol=2, loc='upper left',
          columnspacing=0.3)  #fontsize=10  loc=(1.36/5,0.05/5)
ax.grid()
ax.set_xlim(30, 80)
ax.set_xticks(x)
# ax.set_xticklabels(x)
ax.set_ylim(0, 15)
# ax.set_yscale("log")
# ax.set_yticks([2,4,8,16])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=24)
gcf = plt.gcf()
plt.show()
gcf.savefig('./exp7_Se.pdf', format='pdf', bbox_inches='tight')
