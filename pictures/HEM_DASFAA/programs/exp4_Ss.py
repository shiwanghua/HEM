import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False

Name = ["REIN", "BIOP5DD", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [5,10,15,20,25,30]

# Rein = [2.838326, 3.283254, 4.062079, 5.165873, 6.315778, 7.240721	]
# BIOP5DD = [1.502258, 0.903726, 0.825412, 0.848104, 0.892939, 0.946306]
# Simple = [14.721384, 17.223702, 18.032774, 19.689, 22.721534, 25.550688]
# TAMA = [4.416015, 5.485578, 6.772175, 8.331465, 10.162467, 11.904193]
# AdaREIN = [0.427167, 2.811251, 2.779868, 3.602478, 4.751798, 5.866896]
# OpIndex = [11.974645, 16.94116, 22.58829, 28.596598, 34.314555, 48.606402]

Rein = [2.907546, 3.446939, 4.311283, 5.442733, 6.491765, 7.425057					]
BIOP5DD = [1.476032, 0.889661, 0.819263, 0.840787, 0.940138, 0.936541					
]
Simple = [14.663204, 16.966962, 17.812218, 19.407818, 24.079618, 26.632523					
]
TAMA = [4.615074, 5.989808, 7.941371, 10.010741, 12.595307, 14.992011					
]
AdaREIN = [0.427861, 2.778039, 2.740648, 3.632821, 4.619093, 5.770939					
]
OpIndex = [11.634924, 16.569527, 22.410342, 28.271301, 34.062668, 47.600016					
]


fig=plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('Size of Subscriptions', fontsize=13)
ax.set_ylabel('Matching Time (ms)', fontsize=13)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, BIOP5DD, marker='.', color='DEEPPINK', label=Name[1])
ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend( loc=(1.36/5,0.05/5),ncol=3) #fontsize=10
ax.grid()
ax.set_xlim(5,30)
ax.set_xticks(x)
# ax.set_xticklabels(x)

ax.set_yscale("log",base=4,subs=[2,3])
ax.set_ylim(0.25,52)
# ax.set_yticks([0,2,8,32])
ax.set_yticklabels(['0','0.25','1', '4', '16','64'])
ax.set_zorder(0)

gcf = plt.gcf()
plt.show()
gcf.savefig('../exp4_Ss.eps',format='eps',bbox_inches='tight')