import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False

Name = ["REIN", "BIOP5DD", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [30,40,50,60,70,80]

Rein = [14.189014, 13.809997, 13.482046, 13.053054, 12.485939, 12.015104]
BIOP5DD = [1.392849, 1.552273, 1.673994, 1.81849, 1.962091, 2.06982	]
Simple = [9.441057, 10.222164, 10.923821, 11.553313, 12.191293, 12.362122	]
TAMA = [2.776852, 3.056638, 3.333762, 3.583243, 3.771276, 4.034415	]
AdaREIN = [10.042542, 9.600684, 9.473792, 9.16284, 8.75234, 8.534988	]
OpIndex = [9.512111, 11.428797, 14.28519, 17.002925, 20.309615, 24.193927	]

fig=plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('Size of Events')
ax.set_ylabel('Matching Time (ms)')
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, BIOP5DD, marker='.', color='DEEPPINK', label=Name[1])
ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend(ncol=3) #fontsize=10  loc=(1.36/5,0.05/5)
ax.grid()
ax.set_xlim(30,80)
ax.set_xticks(x)
# ax.set_xticklabels(x)
ax.set_ylim(0,25)
# ax.set_yscale("log")
# ax.set_yticks([2,4,8,16])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)

gcf = plt.gcf()
plt.show()
gcf.savefig('../exp5_Se.eps',format='eps',bbox_inches='tight')