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

# Construction
Rein = [0.000165, 0.000254, 0.000384, 0.000505, 0.00066, 0.000785]
HEM5DD = [0.000265, 0.000378, 0.000529, 0.000645, 0.00085, 0.000977]
HEM5_VAS = [0.000272, 0.000393, 0.000545, 0.000699, 0.000901, 0.001056]
TAMA = [0.00177, 0.003401, 0.005107, 0.00671, 0.010561, 0.010403]
AdaREIN = [0.000162, 0.000261, 0.000397, 0.000501, 0.000668, 0.00077]

Simple = [0.000047, 0.000063, 0.000066, 0.00008, 0.000082, 0.000102]
OpIndex = [0.000077, 0.000126, 0.000142, 0.000199, 0.000292, 0.000338]

mul = 1000  # ms
Rein = np.array(Rein) * mul
HEM5_VAS = np.array(HEM5_VAS) * mul
Simple = np.array(Simple) * mul
TAMA = np.array(TAMA) * mul
AdaREIN = np.array(AdaREIN) * mul
OpIndex = np.array(OpIndex) * mul

lsize = 24

fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Size of Subscriptions', fontsize=lsize)
ax.set_ylabel('Inserting Time (us)', fontsize=lsize, labelpad=0)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM5_VAS, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5])  #   slategray

ax.legend(loc=(1.62 / 5, 2.75 / 5), ncol=2, fontsize=12)  #
ax.grid()
ax.set_xlim(5, 30)
# ax.set_xticks([0,1,2,3,4,5])
ax.set_xticklabels(x)
ax.set_yscale("log", base=10, subs=[2, 3, 4, 5, 6, 7, 8, 9])
ax.set_ylim(0, 12)
# ax.set_yticks([0,2,8,32,128,256])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=18)
gcf = plt.gcf()
plt.show()
gcf.savefig('./exp14_constructingTime.pdf', format='pdf', bbox_inches='tight')
