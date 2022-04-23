import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman']  #
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex", "PS-Tree"]
x = [0, 1, 2, 3, 4]
x_label = ['100', '1k', '10k', '100k', '1M']

HEM0 = [8.432855, 8.136829, 8.210980, 8.102541, 8.385901]
HEM5_RAG = [0.186224, 0.175863, 0.180904, 0.185770, 0.136743]
HEM5_VAG = [0.401715, 0.396702, 0.407294, 0.398994, 0.359286]
Simple = [7.737238, 7.468173, 7.463276, 7.536389, 9.393921]
Rein = [31.661535, 28.859180, 29.058861, 28.830728, 10.970821]
AdaRein_ORI = [28.601099, 28.318395, 28.123900, 28.205075, 11.054627]
HEM5_avxOR = [12.654341, 11.839945, 11.877288, 11.838674, 12.361595]
HEM5 = [9.054133, 8.235714, 8.903678, 8.192088, 8.561228]
Tama = [1.577300, 1.592117, 1.586025, 1.572749, 1.601439]
OpIndex2 = [1.474626, 1.483398, 1.477513, 1.470170, 1.553462]
PSTree = [0.859636, 1.142911, 2.035849, 2.365542, 2.632586]

Rein.reverse()
Simple.reverse()
AdaRein_ORI.reverse()
HEM5.reverse()
Tama.reverse()
OpIndex2.reverse()
PSTree.reverse()

lsize = 24
fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Value Domain', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM5_RAG, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, OpIndex2, marker='h', color='DimGray', label=Name[5])  #   slategray
ax.plot(x, Tama, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaRein_ORI, marker='x', color='DarkMagenta', label=Name[4])
# ax.plot(x, PSTree, marker='1', color='DarkOrange', label=Name[6])  #

ax.legend(fontsize=16, loc=(0.6 / 4, 0.8 / 4), ncol=2,
          columnspacing=0.3)  #fontsize=10
ax.grid()
ax.set_xlim(0, 4)
ax.set_xticks(x)
ax.set_xticklabels(x_label)

# ax.set_yscale("log")
# ax.set_yticks([0,2,8,32,128,256])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=24)
gcf = plt.gcf()
plt.show()
gcf.savefig('./exp12_valDom.pdf', format='pdf', bbox_inches='tight')
