import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
# from matplotlib.pyplot import MultipleLocator
from matplotlib.ticker import MultipleLocator, FormatStrFormatter
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False

Name = ["REIN", "BIOP5DD", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [5,10,15,20,25,30]


# Deletion
Rein = [0.004484, 0.015392, 0.030884, 0.053992, 0.077825, 0.110583					
]
BIOP5DD = [0.005819, 0.017513, 0.034552, 0.057127, 0.090317, 0.117644					
]
Simple = [1.185564, 1.230383, 1.215385, 1.110408, 1.209899, 1.1978					
]
TAMA = [0.401066, 1.790405, 4.731472, 8.660076, 14.058244, 20.636517					
]
AdaREIN = [0.004598, 0.015317, 0.032228, 0.055725, 0.079468, 0.111521					
]
OpIndex = [0.072183, 0.51594, 2.198451, 6.070166, 13.738984, 0.644734					
]	

mul=1 # ms
Rein=np.array(Rein)*mul
BIOP5DD=np.array(BIOP5DD)*mul
Simple=np.array(Simple)*mul
TAMA=np.array(TAMA)*mul
AdaREIN=np.array(AdaREIN)*mul
OpIndex=np.array(OpIndex)*mul

fig=plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('Size of Subscriptions', fontsize=13)
ax.set_ylabel('Deleting Time (ms)', fontsize=13)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, BIOP5DD, marker='.', color='DEEPPINK', label=Name[1])
ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend( ncol=3) #fontsize=10 loc=(1.36/5,0.01/5),
ax.grid()
ax.set_xlim(5,30)
# ax.set_xticks([0,1,2,3,4,5])
ax.set_xticklabels(x)
# ax.set_yscale("log")
ax.set_yscale("log",base=10,subs=[2,3,4,5,6,7,8,9])
ax.set_ylim(0,22)
# ax.set_yticks([0,2,4,8,16])
# ax.set_yticklabels([0,2,4,8,16])
ax.set_zorder(0)

# xmajorLocator   = MultipleLocator(1)
# ax.xaxis.set_major_locator(xmajorLocator)
# xminorLocator   = MultipleLocator(0.25)
# ax.xaxis.set_minor_locator(xminorLocator)

# ymajorLocator   = MultipleLocator(2)
# ax.yaxis.set_major_locator(ymajorLocator)
# yminorLocator   = MultipleLocator(1)
# ax.yaxis.set_minor_locator(yminorLocator)

gcf = plt.gcf()
plt.show()
gcf.savefig('../exp10_deletion.eps',format='eps',bbox_inches='tight')