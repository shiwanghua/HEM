import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.ticker import FuncFormatter
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')
plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman']  #
# plt.rcParams.update({'font.size': 8})
plt.figure(figsize=(5, 4))
be = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512]
width = 0.35
Name = ["Comparison", "Mark", "BitOR", "Check"]

Comparison = [
    0.044794, 0.043918, 0.043875, 0.043762, 0.044577, 0.044838, 0.045258,
    0.044868, 0.045988, 0.045245
]
Mark = [
    2.815691, 1.387544, 0.728088, 0.391327, 0.229163, 0.143344, 0.09613,
    0.068335, 0.049382, 0.015175
]
BitOR = [
    0.085204, 0.130256, 0.153324, 0.161091, 0.176804, 0.172563, 0.170424,
    0.175759, 0.210117, 0.203333
]
Check = [
    0.006874, 0.006807, 0.006825, 0.006784, 0.006789, 0.006808, 0.006775,
    0.006767, 0.006772, 0.006782
]

data = [Comparison, Mark, BitOR, Check]
x = np.arange(10)
# data=np.array(data).transpose()
sums = [sum(i) for i in np.array(data).transpose()]
bottom_y = [0] * len(be)
i = 0
# print(len(data),len(sums))
Color = ['pink', 'DODGERBLUE', 'lightblue', 'r']
for d in data:
    y = [a / b for a, b in zip(d, sums)]
    #  print(y)
    plt.bar(x, y, width, bottom=bottom_y, color=Color[i], label=Name[i])
    i += 1
    bottom_y = [(a + b) for a, b in zip(y, bottom_y)]

# plt.bar(be, BIOP5DD, width, label=Name[6],color='r',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b4, width, bottom=BIOP5DD, label=Name[5],color='y',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b3, width, bottom=BIOP2SD, label=Name[4],color='m',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b2, width, bottom=BIOP4DS, label=Name[3],color='c',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b1, width, bottom=BIOP3PD, label=Name[2],color='g',alpha=0.5,ec='k',lw=.6)
# plt.bar(be, b0, width,bottom=BIOP1SS, label=Name[1],color='b',alpha=0.5,ec='k',lw=.6) # hatch="//",


def to_percent(temp, position):
    return '%1.0f' % (100 * temp) + '%'


plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))

plt.xticks(x, labels=be)
plt.yticks(np.arange(0, 1.01, 0.1))
plt.xlabel('Number of Groups', fontsize=16)  # ,fontsize=13
plt.ylabel('Time Proportion (%)', fontsize=16)  # ,fontsize=13
plt.tick_params(direction='out',
                labelsize=14,
                length=4,
                width=1,
                top=False,
                right=False)
plt.legend(
    fontsize=13, loc=(-0.15, 1.01), ncol=4, columnspacing=0.3
)  # labelspacing=0.4, fontsize=6,frameon=False,loc='upper center',ncol=6
# # ax.text(.87,-.08,'\nVisualization by DataCharm',transform = ax.transAxes,
# #         ha='center', va='center',fontsize = 5,color='black',fontweight='bold',family='Roboto Mono')

gcf = plt.gcf()
plt.show()
gcf.savefig('./exp1_HEM5TimeDistribution_be.pdf',
            format='pdf',
            bbox_inches='tight')
