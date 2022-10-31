# coding=gbk
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')
plt.rcParams['font.family'] = ['Times New Roman']  #
plt.rcParams['axes.unicode_minus'] = False

x_label = ["20", "50", "100", "400", "500", "1000"]
x = np.arange(6)  #x轴刻度标签位置
Name = ["HEM-LD-VAS", "HEM-LD-RAS"]

HEM5_VAG = [53.276685, 26.959339, 15.227813, 7.188751, 7.167826, 9.302589]

HEM5_RAG = [4.026518, 1.565677, 0.787057, 0.159268, 0.118743, 0.066751]

fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)

# ax.plot(be, Rein, marker='o', color='r', label=Name[0])
# ax.plot(x, HEM, marker='x', color='DODGERBLUE', label=Name[1])
# ax.plot(be, BIOP1SS, marker='+', color='g', label=Name[2])
# ax.plot(be, BIOP2SD, marker='*', color='b', label=Name[3])
# ax.plot(be, BIOP3PD, marker='^', color='m', label=Name[4])
# ax.plot(be, BIOP4DS, marker='s', color='y', label=Name[5])
# ax.plot(be, BIOP5DD, marker='.', color='DEEPPINK', label=Name[6])
# ax.plot(be, Simple, marker='D',  color='lightseagreen', label=Name[7])
ax.plot(x, HEM5_VAG, marker='s', color='#DB7093', label=Name[0])
ax.plot(x, HEM5_RAG, marker='^', color='#9400D3', label=Name[1])  # alpha=0.7,

lgdsize = 15
lsize = 24
tickSize=20
ax.legend(loc='lower left', fontsize=lgdsize,ncol=1) # (3.3 / 10, 2.5 / 5)
ax.grid()
ax.set_xlabel('Number of Attribute Subsets', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
ax.set_yscale("log",base=4, subs=[2, 3])
ax.set_ylim(0, 65)
ax.set_yticks([0.25, 1, 4, 16, 64])
ax.set_yticklabels(['0.25', '1', '4','16', '64'])
# plt.xticks(range(0,10))
ax.set_xlim(-0.5, 4.5)
ax.set_zorder(0)
# x_major_locator = MultipleLocator(1)
# ax.xaxis.set_major_locator(x_major_locator)
# for size in ax.get_xticklabels():  #获取x轴上所有坐标，并设置字号
#     # size.set_fontname('Times New Roman')
#     size.set_fontsize('16')
plt.tick_params(labelsize=tickSize)
# plt.tick_params(direction='out', labelsize=tickSize, length=4, width=1)
# for size in ax2.get_xticklabels():   #获取x轴上所有坐标，并设置字号
#     # size.set_fontname('Times New Roman')
#     size.set_fontsize('16')
plt.xticks(x, labels=x_label)
# plt.tick_params(direction='out', labelsize=tickSize, length=4, width=1)
# for a, b in zip(x, Memory):
#     c = b - 160
#     if a < 5:
#         c += 20
#     plt.text(a, c, '%.0f' % b, ha='center', va='bottom')  #,fontsize=7

gcf = plt.gcf()
plt.show()
gcf.savefig('./exp3_4_matchingTime_VRAS2.pdf',
            format='pdf',
            bbox_inches='tight')



# 双坐标轴
# coding=gbk
# import matplotlib
# import matplotlib.pyplot as plt
# import numpy as np
# from matplotlib import rc
# from matplotlib.pyplot import MultipleLocator

# rc('mathtext', default='regular')
# plt.rcParams['font.family'] = ['Times New Roman']  #
# plt.rcParams['axes.unicode_minus'] = False

# x_label = ["20", "50", "100", "400", "500", "1000"]
# x = np.arange(6)  #x轴刻度标签位置
# Name = ["HEMDD-VAS", "HEMDD-RAS"]

# HEM5_VAG = [53.276685, 26.959339, 15.227813, 7.188751, 7.167826, 9.302589]

# HEM5_RAG = [4.026518, 1.565677, 0.787057, 0.159268, 0.118743, 0.066751]

# fig = plt.figure(figsize=(5, 4))
# ax = fig.add_subplot(111)

# # ax.plot(be, Rein, marker='o', color='r', label=Name[0])
# # ax.plot(x, HEM, marker='x', color='DODGERBLUE', label=Name[1])
# # ax.plot(be, BIOP1SS, marker='+', color='g', label=Name[2])
# # ax.plot(be, BIOP2SD, marker='*', color='b', label=Name[3])
# # ax.plot(be, BIOP3PD, marker='^', color='m', label=Name[4])
# # ax.plot(be, BIOP4DS, marker='s', color='y', label=Name[5])
# # ax.plot(be, BIOP5DD, marker='.', color='DEEPPINK', label=Name[6])
# # ax.plot(be, Simple, marker='D',  color='lightseagreen', label=Name[7])
# ax.plot(x, HEM5_VAG, marker='s', color='#DB7093', label=Name[0])

# lgdsize = 18
# lsize = 24
# tickSize=18
# ax.legend(loc=(3 / 10, 3.9 / 5), fontsize=lgdsize)
# ax.grid()
# ax.set_xlabel('Number of Attribute Subset', fontsize=22)
# ax.set_ylabel('Matching Time of VAS (ms)', fontsize=lsize)
# # plt.xticks(range(0,10))
# ax.set_xlim(-0.5, 4.5)
# ax.set_zorder(0)
# # x_major_locator = MultipleLocator(1)
# # ax.xaxis.set_major_locator(x_major_locator)
# # for size in ax.get_xticklabels():  #获取x轴上所有坐标，并设置字号
# #     # size.set_fontname('Times New Roman')
# #     size.set_fontsize('16')
# plt.tick_params(labelsize=tickSize)
# # plt.tick_params(direction='out', labelsize=tickSize, length=4, width=1)
# ax2 = ax.twinx()
# ax2.plot(x, HEM5_RAG, marker='^', color='#9400D3', label=Name[1])  # alpha=0.7,
# ax2.set_ylabel("Matching Time of RAS (ms)", fontsize=lsize)
# ax2.set_ylim(0, 4.2)
# ax2.legend(loc=(3 / 10, 2.8 / 5), fontsize=lgdsize, ncol=1)
# ax2.set_zorder(1)
# # for size in ax2.get_xticklabels():   #获取x轴上所有坐标，并设置字号
# #     # size.set_fontname('Times New Roman')
# #     size.set_fontsize('16')
# plt.xticks(x, labels=x_label)
# plt.tick_params(labelsize=tickSize)
# # plt.tick_params(direction='out', labelsize=tickSize, length=4, width=1)
# # for a, b in zip(x, Memory):
# #     c = b - 160
# #     if a < 5:
# #         c += 20
# #     plt.text(a, c, '%.0f' % b, ha='center', va='bottom')  #,fontsize=7

# gcf = plt.gcf()
# plt.show()
# gcf.savefig('./exp3_4_matchingTime_VRAS.pdf',
#             format='pdf',
#             bbox_inches='tight')

