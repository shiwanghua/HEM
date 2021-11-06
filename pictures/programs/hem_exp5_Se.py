import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False

Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [30,40,50,60,70,80]

Rein = [14.373395, 14.010372, 13.600084, 13.139275, 12.795341, 12.364449					
]
HEM = [1.371466, 1.43086, 1.64819, 1.858077, 2.055626, 2.308927					
]
Simple = [9.174505, 10.139296, 10.930411, 11.616887, 12.534007, 13.218141					
]
TAMA = [3.06763, 3.097819, 3.296786, 3.615934, 3.897723, 4.321846					
]
AdaREIN = [14.382465, 13.749294, 13.252763, 12.87528, 12.639115, 11.999519					
]
OpIndex = [9.475935, 11.562274, 14.261331, 17.027247, 20.460719, 24.468937					
]
lsize=20

fig=plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('Size of Events', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend( fontsize=14,ncol=2) #fontsize=10  loc=(1.36/5,0.05/5)
ax.grid()
ax.set_xlim(30,80)
ax.set_xticks(x)
# ax.set_xticklabels(x)
ax.set_ylim(0,25)
# ax.set_yscale("log")
# ax.set_yticks([2,4,8,16])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=15)
gcf = plt.gcf()
plt.show()
gcf.savefig('../exp5_Se.eps',format='eps',bbox_inches='tight')