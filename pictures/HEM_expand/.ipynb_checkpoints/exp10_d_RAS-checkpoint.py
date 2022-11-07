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

# HEM5_VAS = [4.078117, 1.020156, 0.412368, 0.195293, 0.133136, 0.185654]
# Rein = [51.274168, 36.845447, 21.397414, 12.373126, 6.689029, 2.931841]
# AdaRein_ORI = [49.796203, 33.130457, 20.298914, 11.045674, 6.183894, 2.852660]
# HEM5_RAS = [0.104509, 0.112586, 0.122237, 0.118615, 0.113712, 0.190168]
# Tama = [1.509458, 1.458342, 1.696168, 1.981427, 2.539738, 3.202192]
# OpIndex2 = [1.136534, 1.211228, 1.589342, 2.535680, 4.383471, 6.125082]

# Rein.reverse()
# # Simple.reverse()
# AdaRein_ORI.reverse()
# HEM5_RAS.reverse()
# Tama.reverse()
# OpIndex2.reverse()

# lsize = 24
# fig = plt.figure(figsize=(5, 4))
# ax = fig.add_subplot(111)
# ax.set_xlabel('Number of Attributes', fontsize=lsize)
# ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# # plt.xticks(range(0,10))
# ax.plot(x, Rein, marker='v', color='r', label=Name[0])
# ax.plot(x, HEM5_RAS, marker='.', color='DODGERBLUE', label=Name[1])
# # ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
# ax.plot(x, Tama, marker='*', color='DarkCyan', label=Name[3])
# ax.plot(x, AdaRein_ORI, marker='x', color='DarkMagenta', label=Name[4])
# ax.plot(x, OpIndex2, marker='h', color='DimGray', label=Name[5])  #   slategray

# ax.legend(fontsize=11, ncol=2, loc=(0.1 / 5, 0.8 / 5))  #'lower right'
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
# plt.tick_params(labelsize=24)
# gcf = plt.gcf()
# plt.show()
# gcf.savefig('./exp10_d_RAS.pdf', format='pdf', bbox_inches='tight')



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

HEM5_VAS = [4.078117, 1.020156, 0.412368, 0.195293, 0.133136, 0.185654]
Rein = [51.274168, 36.845447, 21.397414, 12.373126, 6.689029, 2.931841]
AdaRein_ORI = [49.796203, 33.130457, 20.298914, 11.045674, 6.183894, 2.852660]
HEM5_RAS = [0.104509, 0.112586, 0.122237, 0.118615, 0.113712, 0.190168]
Tama = [1.509458, 1.458342, 1.696168, 1.981427, 2.539738, 3.202192]
OpIndex2 = [1.136534, 1.211228, 1.589342, 2.535680, 4.383471, 6.125082]

Rein.reverse()
# Simple.reverse()
AdaRein_ORI.reverse()
HEM5_RAS.reverse()
Tama.reverse()
OpIndex2.reverse()

lsize = 24
fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Number of Attribute Classes', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM5_RAS, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, Tama, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaRein_ORI, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex2, marker='h', color='DimGray', label=Name[5])  #   slategray

ax.legend(fontsize=11, ncol=2, loc=(0.1 / 5, 0.8 / 5))  #'lower right'
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
plt.tick_params(labelsize=24)
gcf = plt.gcf()
plt.show()
gcf.savefig('./exp10_d_RAS_biye.pdf', format='pdf', bbox_inches='tight')
