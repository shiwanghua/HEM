import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman'] # 
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [5,10,15,20,25,30]


Rein = [3.262534, 4.900928, 6.608908, 7.966946, 9.469752, 11.089413					
]
HEM = [0.707932, 0.874511, 1.034393, 1.180242, 1.338893, 1.510915								
]
Simple = [10.692779, 12.521723, 14.346012, 17.088473, 19.285018, 20.490553					
			
]
TAMA = [3.291131, 4.380067, 5.703838, 7.317489, 9.074819, 10.773498					
					
]
AdaREIN = [3.021985, 4.700881, 6.42731, 7.79122, 9.242951, 10.924326										
]
OpIndex = [9.652425, 16.21203, 22.26091, 29.071672, 36.013989, 45.053037										
]

lsize=20

fig=plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('Size of Subscriptions', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend( fontsize=13,ncol=3) #fontsize=10 , loc=(1.36/5,0.05/5)
ax.grid()
ax.set_xlim(5,30)
ax.set_xticks(x)
# ax.set_xticklabels(x)

ax.set_yscale("log",base=4,subs=[2,3])
ax.set_ylim(0.25,52)
# ax.set_yticks([0,2,8,32])
ax.set_yticklabels(['0','0.25','1', '4', '16','64'])
ax.set_zorder(0)
plt.tick_params(labelsize=15)
gcf = plt.gcf()
plt.show()
gcf.savefig('../exp4_Ss.eps',format='eps',bbox_inches='tight')