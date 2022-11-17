import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman']  #
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [5, 10, 15, 20, 25, 30]

Rein = [6.928553, 5.905597, 4.924651, 3.907403, 3.009564, 2.670325]
AdaREIN = [6.968948, 5.934843, 4.955735, 4.051646, 3.267344, 2.687540]
HEM = [0.474652, 0.429097, 0.368811, 0.315633, 0.255439, 0.197971]
TAMA = [12.798376, 11.134401, 9.572079, 7.582473, 5.660134, 4.948571]
OpIndex = [49.758897, 36.853753, 30.202386, 23.727563, 17.570755, 12.638957]

Rein.reverse()
AdaREIN.reverse()
HEM.reverse()
TAMA.reverse()
OpIndex.reverse()

lsize = 24

fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Size of Subscriptions', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5])  #   slategray

ax.legend(fontsize=11, ncol=3, loc=(0.3 / 5, 1.55 / 5))  #fontsize=10 ,
ax.grid()
ax.set_xlim(5, 30)
ax.set_xticks(x)
# ax.set_xticklabels(x)

ax.set_yscale("log", base=4, subs=[2, 3])
ax.set_ylim(0.1, 52)
# ax.set_yticks([0,2,8,32])
ax.set_yticklabels(['0', '0', '0.25', '1', '4', '16', '64'])
ax.set_zorder(0)
plt.tick_params(labelsize=22)
gcf = plt.gcf()
plt.show()
gcf.savefig('./exp6_Ss.pdf', format='pdf', bbox_inches='tight')
