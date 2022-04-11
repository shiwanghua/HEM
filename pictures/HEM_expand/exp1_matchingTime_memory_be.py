import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')
plt.rcParams['font.family'] = ['Times New Roman']  #
plt.rcParams['axes.unicode_minus'] = False

g = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512]
x = np.arange(10)  # x轴刻度标签位置
Name = [
    "Rein", "HEM0PS", "BIOP1SS", "BIOP2SD", "BIOP3PD", "BIOP4DS", "HEM5DD",
    "Simple", "Simple2"
]

HEM0PS = [
    6.24005, 4.153866, 2.312664, 1.507056, 0.924996, 0.672416, 0.535015,
    0.458862, 0.439522, 0.405026
]
HEM5DD = [
    2.962161, 1.57773, 0.940414, 0.610254, 0.464354, 0.374525, 0.325231,
    0.301888, 0.318704, 0.276994
]
Memory = [154, 161, 170, 189, 227, 304, 456, 761, 1372, 2593]

fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111)

# ax.plot(be, Rein, marker='o', color='r', label=Name[0])
ax.plot(x, HEM0PS, marker='x', color='DODGERBLUE', label=Name[1])
# ax.plot(be, BIOP1SS, marker='+', color='g', label=Name[2])
# ax.plot(be, BIOP2SD, marker='*', color='b', label=Name[3])
# ax.plot(be, BIOP3PD, marker='^', color='m', label=Name[4])
# ax.plot(be, BIOP4DS, marker='s', color='y', label=Name[5])
ax.plot(x, HEM5DD, marker='.', color='DEEPPINK', label=Name[6])
# ax.plot(be, Simple, marker='D',  color='lightseagreen', label=Name[7])
lgdsize = 18
lsize = 24
ax.legend(loc=(2.7 / 10, 4.1 / 5), fontsize=lgdsize)
ax.grid()
ax.set_xlabel('Number of Groups', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.set_xlim(-0.5, 9.5)
ax.set_zorder(0)
x_major_locator = MultipleLocator(1)
ax.xaxis.set_major_locator(x_major_locator)
for size in ax.get_xticklabels():  #获取x轴上所有坐标，并设置字号
    # size.set_fontname('Times New Roman')
    size.set_fontsize('16')
plt.tick_params(direction='out', labelsize=18, length=4, width=1)
ax2 = ax.twinx()
ax2.bar(x, Memory, color='lightsteelblue', label='Memory (MB)')  # alpha=0.7,
ax2.set_ylabel(r"Memory Size of HEM5DD", fontsize=lsize)
ax2.set_ylim(0, 2700)
ax2.legend(loc=(2.7 / 10, 3 / 5), fontsize=lgdsize, ncol=1)
ax2.set_zorder(1)
# for size in ax2.get_xticklabels():   #获取x轴上所有坐标，并设置字号
#     # size.set_fontname('Times New Roman')
#     size.set_fontsize('16')
plt.xticks(x, labels=g)
plt.tick_params(direction='out', labelsize=18, length=4, width=1)
for a, b in zip(x, Memory):
    c = b - 100
    if a == 5:
        c -= 10
    elif a == 4:
        c -= 10
    plt.text(a, c, '%.0f' % b, ha='center', va='bottom')  #,fontsize=7

gcf = plt.gcf()
plt.show()
gcf.savefig('./exp1_matchingTime_memory_be.pdf',
            format='pdf',
            bbox_inches='tight')
