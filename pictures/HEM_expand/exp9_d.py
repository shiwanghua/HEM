import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman']  #
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = ["30", "100", "300", "500", "700", "900"]

Rein = [7.525526, 13.342468, 22.322126, 28.495606, 32.916958, 35.535869]
HEM = [0.717611, 1.247773, 3.053349, 5.159292, 7.112203, 9.023087]
Simple = [12.247347, 9.231333, 8.658939, 8.905944, 8.822599, 9.246034]
TAMA = [3.140881, 2.491274, 2.188, 2.404905, 2.097253, 2.166385]
AdaREIN = [7.527089, 13.347175, 22.404558, 27.9598, 31.936498, 35.156202]
OpIndex = [11.292704, 7.286157, 6.127477, 5.944046, 5.845625, 5.932912]
lsize = 24
fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Number of Dimensions', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5])  #   slategray

ax.legend(fontsize=11.7, ncol=2, loc='lower right')
ax.grid()
ax.set_xlim(0, 5)
ax.set_xticks([0, 1, 2, 3, 4, 5])
ax.set_xticklabels(x)
ax.set_yscale("log", base=2, subs=[])
ax.set_ylim(0.5, 40)
# ax.set_yticks([0,2,8,32])
ax.set_yticklabels(['0', '0.5', '1', '2', '4', '8', '16', '32'])
# ax.set_yticks([0,2,8,32,128,256])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=22)
gcf = plt.gcf()
plt.show()
gcf.savefig('../exp7_d.eps', format='eps', bbox_inches='tight')
