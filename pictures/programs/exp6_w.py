import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False

Name = ["REIN", "BIOP5DD", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [0.1,0.2,0.3,0.4,0.5,0.6,0.7]

Rein = [6.559427, 5.557767, 4.73157, 3.889393, 3.268577, 2.826192, 2.601284]
BIOP5DD = [1.030988, 0.958024, 0.897166, 0.865233, 0.899745, 1.049251, 1.415623]
Simple = [9.271855, 11.049785, 13.031425, 14.331424, 14.951854, 14.989832, 14.623044	]
TAMA = [2.209476, 2.562456, 2.893346, 3.29616, 3.394748, 3.788076, 4.371151]
AdaREIN = [5.1065, 4.297373, 3.162723, 2.581329, 2.882192, 1.494825, 0.42953]
OpIndex = [12.971666, 13.176297, 12.826006, 12.108869, 11.550296, 11.162329, 11.053504]


fig=plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('Number of Predicate Widths')
ax.set_ylabel('Matching Time (ms)')
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, BIOP5DD, marker='.', color='DEEPPINK', label=Name[1])
ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend( ncol=3) #fontsize=10 loc=(1.36/5,0.05/5),
ax.grid()
ax.set_xlim(0.1,0.7)
ax.set_xticks(x)
# ax.set_xticklabels(x)
# ax.set_yscale("log")
ax.set_ylim(0,16)
# ax.set_yticks([0,3,6,9,12,15])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)

gcf = plt.gcf()
plt.show()
gcf.savefig('../exp6_w.eps',format='eps',bbox_inches='tight')

