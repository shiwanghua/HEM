import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False

Name = ["REIN", "BIOP5DD", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9]

Rein = [6.478509, 5.855915, 5.147803, 4.576884, 4.00896, 3.491413, 2.888849, 2.548706, 2.631894					
]
BIOP5DD = [0.393996, 0.397894, 0.383746, 0.389825, 0.404568, 0.431202, 0.459177, 0.502099, 0.554493					
]
Simple = [7.962312, 9.271659, 10.361115, 11.090305, 11.958599, 12.943751, 13.885826, 14.571651, 13.354723					
]
TAMA = [2.050141, 2.298467, 2.510315, 2.581851, 2.854773, 2.922369, 3.204974, 3.706224, 4.935104								
]
AdaREIN = [6.252634, 5.618747, 4.911578, 4.339218, 3.792963, 3.269768, 2.688518, 2.403031, 2.488923					
]
OpIndex = [10.698055, 10.208228, 9.910692, 10.159506, 10.383417, 10.356466, 10.134858, 10.778922, 11.532879							
]

lsize=16

fig=plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('Number of Predicate Widths', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, BIOP5DD, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend( fontsize=10, ncol=3) #fontsize=10 loc=(1.36/5,0.05/5),
ax.grid()
ax.set_xlim(0.1,0.9)
ax.set_xticks(x)
# ax.set_xticklabels(x)
# ax.set_yscale("log")
ax.set_ylim(0,16)
# ax.set_yticks([0,3,6,9,12,15])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=13)
gcf = plt.gcf()
plt.show()
gcf.savefig('../exp6_w.eps',format='eps',bbox_inches='tight')

