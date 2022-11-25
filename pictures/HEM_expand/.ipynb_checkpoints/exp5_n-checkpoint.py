import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman']  #
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = ["0.3M", "1M", "3M", "5M", "7M", "9M"]

Rein = [1.820763, 7.085707, 19.885283, 35.199368, 50.704338, 70.720695]
HEM = [0.088271, 0.310228, 0.980195, 1.790819, 3.043679, 8.06583]
TAMA = [0.892801, 3.896153, 12.482383, 25.350876, 48.453852, 76.326296]
AdaRein = [1.771185, 7.530486, 18.873681, 33.771292, 51.144931, 69.162622]
OpIndex = [5.785857, 19.654474, 58.63391, 101.669787, 161.303878, 230.510626]

lsize = 24

fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Number of Subscriptions', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaRein, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5])  #   slategray

ax.legend(fontsize=12, ncol=2,
          loc='lower right')  #fontsize=10 loc=(1.36/5,0.05/5),
ax.grid()
ax.set_xlim(0, 5)
ax.set_xticks([0, 1, 2, 3, 4, 5])
ax.set_xticklabels(x)
ax.set_yscale("log")
# ax.set_ylim(0, 250)
# ax.set_yticks([0, 0.1, 1, 10, 100, 1000])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=22)
gcf = plt.gcf()
plt.show()
gcf.savefig('./exp5_n.pdf', format='pdf', bbox_inches='tight')
