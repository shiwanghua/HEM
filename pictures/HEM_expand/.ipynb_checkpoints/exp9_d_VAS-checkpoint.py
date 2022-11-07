# coding=gbk
# import matplotlib
# import matplotlib.pyplot as plt
# import numpy as np
# from matplotlib import rc
# from matplotlib.pyplot import MultipleLocator

# rc('mathtext', default='regular')

# plt.rcParams['axes.unicode_minus'] = False
# plt.rcParams['font.family'] = ['Times New Roman']  #
# Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
# x = ["30", "100", "300", "1k", "3k", "10k"]

# HEM = [7.295973, 3.713577, 2.102555, 0.966343, 0.402103, 0.182992]
# Simple = [18.883054, 12.119494, 6.735993, 5.690750, 6.650945, 15.245289]
# Rein = [51.559147, 33.450221, 20.636215, 10.640785, 5.474277, 2.662649]
# AdaREIN = [47.341016, 32.124697, 19.574878, 10.215780, 5.246019, 2.621369]
# TAMA = [1.402612, 1.456788, 1.523113, 1.745815, 2.187047, 3.118311]
# OpIndex = [1.018494, 1.065417, 1.129113, 1.413666, 2.117866, 5.711690]

# Rein.reverse()
# Simple.reverse()
# AdaREIN.reverse()
# HEM.reverse()
# TAMA.reverse()
# OpIndex.reverse()

# lsize = 24
# fig = plt.figure(figsize=(5, 4))
# ax = fig.add_subplot(111)
# ax.set_xlabel('Number of Attributes', fontsize=lsize)
# ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# # plt.xticks(range(0,10))
# ax.plot(x, Rein, marker='v', color='r', label=Name[0])
# ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# # ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
# ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
# ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
# ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5])  #   slategray

# ax.legend(fontsize=11.5, ncol=2, loc='lower right')
# ax.grid()
# ax.set_xlim(0, 5)
# ax.set_xticks([0, 1, 2, 3, 4, 5])
# ax.set_xticklabels(x)
# ax.set_yscale("log", base=4, subs=[2, 3])
# # ax.set_ylim(0, 60)
# ax.set_yticks([0.25, 1, 4, 16, 64])
# ax.set_yticklabels(['0.25', '1', '4', '16', '64'])

# # ax.set_yticks([0,2,8,32,128,256])
# # ax.set_yticklabels(['-1', '0', '1'])
# ax.set_zorder(0)
# plt.tick_params(labelsize=22)
# gcf = plt.gcf()
# plt.show()
# gcf.savefig('./exp9_d_VAS.pdf', format='pdf', bbox_inches='tight')



import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman']  #
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = ["30", "100", "300", "1k", "3k", "10k"]

HEM = [7.295973, 3.713577, 2.102555, 0.966343, 0.402103, 0.182992]
Simple = [18.883054, 12.119494, 6.735993, 5.690750, 6.650945, 15.245289]
Rein = [51.559147, 33.450221, 20.636215, 10.640785, 5.474277, 2.662649]
AdaREIN = [47.341016, 32.124697, 19.574878, 10.215780, 5.246019, 2.621369]
TAMA = [1.402612, 1.456788, 1.523113, 1.745815, 2.187047, 3.118311]
OpIndex = [1.018494, 1.065417, 1.129113, 1.413666, 2.117866, 5.711690]

Rein.reverse()
Simple.reverse()
AdaREIN.reverse()
HEM.reverse()
TAMA.reverse()
OpIndex.reverse()

lsize = 24
fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Number of Attribute Classes', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5])  #   slategray

ax.legend(fontsize=11.5, ncol=2, loc='lower right')
ax.grid()
ax.set_xlim(0, 5)
ax.set_xticks([0, 1, 2, 3, 4, 5])
ax.set_xticklabels(x)
ax.set_yscale("log", base=4, subs=[2, 3])
# ax.set_ylim(0, 60)
ax.set_yticks([0.25, 1, 4, 16, 64])
ax.set_yticklabels(['0.25', '1', '4', '16', '64'])

# ax.set_yticks([0,2,8,32,128,256])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=22)
gcf = plt.gcf()
plt.show()
gcf.savefig('./exp9_d_VAS_biye.pdf', format='pdf', bbox_inches='tight')
