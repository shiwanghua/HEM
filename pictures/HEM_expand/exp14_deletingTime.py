import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
# from matplotlib.pyplot import MultipleLocator
from matplotlib.ticker import MultipleLocator, FormatStrFormatter

rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman']  #
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [5, 10, 15, 20, 25, 30]

# Deletion
Rein = [0.005311, 0.018142, 0.038255, 0.06831, 0.100127, 0.147233]
HEM5DD = [0.006488, 0.020792, 0.042179, 0.070735, 0.106108, 0.147778]
HEM5_VAS = [0.006647, 0.021293, 0.044461, 0.072869, 0.111524, 0.155858]
TAMA = [0.628874, 2.782717, 6.655962, 12.237263, 19.474132, 28.560867]
AdaREIN = [0.005446, 0.01934, 0.040243, 0.067881, 0.10472, 0.148732]
Simple = [1.087727, 1.100354, 1.101376, 1.103331, 1.101528, 1.163857]
OpIndex = [0.062944, 0.44489, 1.795943, 4.787199, 11.153163, 0.575641]

mul = 1  # ms
Rein = np.array(Rein) * mul
HEM5_VAS = np.array(HEM5_VAS) * mul
Simple = np.array(Simple) * mul
TAMA = np.array(TAMA) * mul
AdaREIN = np.array(AdaREIN) * mul
OpIndex = np.array(OpIndex) * mul

fig = plt.figure(figsize=(5, 4))
lsize = 24
ax = fig.add_subplot(111)
ax.set_xlabel('Size of Subscriptions', fontsize=lsize)
ax.set_ylabel('Deleting Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM5_VAS, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5])  #   slategray

ax.legend(fontsize=10, loc='lower right',
          ncol=2)  #fontsize=10 loc=(1.36/5,0.01/5),
ax.grid()
ax.set_xlim(5, 30)
# ax.set_xticks([0,1,2,3,4,5])
ax.set_xticklabels(x)
# ax.set_yscale("log")
ax.set_yscale("log", base=10, subs=[2, 3, 4, 5, 6, 7, 8, 9])
ax.set_ylim(0, 32)
ax.set_yticks([0.01, 0.1, 1, 10])
# ax.set_yticklabels(['0.01', '0.1', '1', '10'])
ax.set_zorder(0)

# xmajorLocator   = MultipleLocator(1)
# ax.xaxis.set_major_locator(xmajorLocator)
# xminorLocator   = MultipleLocator(0.25)
# ax.xaxis.set_minor_locator(xminorLocator)

# ymajorLocator   = MultipleLocator(2)
# ax.yaxis.set_major_locator(ymajorLocator)
# yminorLocator   = MultipleLocator(1)
# ax.yaxis.set_minor_locator(yminorLocator)
plt.tick_params(labelsize=24)
gcf = plt.gcf()
plt.show()
gcf.savefig('./exp14_deletingTime.pdf', format='pdf', bbox_inches='tight')
