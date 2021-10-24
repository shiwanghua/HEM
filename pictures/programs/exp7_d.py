import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False

Name = ["REIN", "BIOP5DD", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = ["30","100","300","500","700","900"]

Rein = [6.727919, 13.056477, 22.288083, 29.771722, 33.56509, 35.514039]
BIOP5DD = [0.90071, 1.277707, 3.181814, 5.059889, 6.922739, 8.917468]
Simple = [12.247347, 9.231333, 8.658939, 8.905944, 8.822599, 9.246034]
TAMA = [3.650443, 2.749272, 2.359933, 2.210257, 2.271962, 2.13714]
AdaREIN = [3.905011, 5.822054, 8.650139, 10.481318, 11.982515, 12.215211]
OpIndex = [11.821977, 6.743832, 5.930406, 5.65893, 5.748188, 5.659542]
					
fig=plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('Number of Dimensions', fontsize=13)
ax.set_ylabel('Matching Time (ms)', fontsize=13)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, BIOP5DD, marker='.', color='DEEPPINK', label=Name[1])
ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend( ncol=3) #fontsize=10 loc=(1.36/5,0.05/5),
ax.grid()
ax.set_xlim(0,5)
ax.set_xticks([0,1,2,3,4,5])
ax.set_xticklabels(x)
ax.set_yscale("log")
# ax.set_yticks([0,2,8,32,128,256])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)

gcf = plt.gcf()
plt.show()
gcf.savefig('../exp7_d.eps',format='eps',bbox_inches='tight')