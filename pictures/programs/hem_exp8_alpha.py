import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman'] # 
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [0,1,2,3,4,5]

Rein = [9.292778, 7.752816, 5.745078, 5.255491, 4.715628, 4.540388					
]
HEM = [0.684095, 0.6901, 0.688694, 0.677551, 0.64799, 0.633782					
]
Simple = [5.543731, 7.255267, 7.526561, 6.346698, 5.655632, 5.359875					
]
TAMA = [1.942452, 2.013692, 2.126841, 2.17193, 2.102254, 2.1022					
]
AdaREIN = [9.069874, 7.438771, 5.4714, 4.701095, 4.30008, 4.122378					
]
OpIndex = [3.981058, 4.508727, 6.888271, 8.516064, 8.77222, 8.579291					
]	
lsize=20
fig=plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('Value of Alpha', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend( fontsize=13,loc=(0.5/5,1.17/5), ncol=3) #fontsize=10 
ax.grid()
ax.set_xlim(0,5)
ax.set_xticks([0,1,2,3,4,5])
ax.set_xticklabels(x)
# ax.set_yscale("log")
# ax.set_yticks([0,2,8,32,128,256])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=15)
gcf = plt.gcf()
plt.show()
gcf.savefig('../exp8_alpha.eps',format='eps',bbox_inches='tight')