import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.ticker import FuncFormatter # 百分比
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')
# 解决坐标轴负号问题
plt.rcParams['axes.unicode_minus'] = False
# plt.rcParams['font.family'] = ['normal'] # Times New Roman
# plt.rcParams.update({'font.size': 8})

be = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
width = 0.35
Name = ["Comparison", "Mark", "BitOr", "Count"]

Comparison = [0.046037, 0.045665, 0.045773, 0.045806, 0.046539, 0.046754, 0.047407, 0.047677, 0.048038, 0.047763]
Mark = [2.608424, 1.817173, 0.957682, 0.515203, 0.303935, 0.187489, 0.12812, 0.092957, 0.065522, 0.029597]
BitOr = [0.17127, 0.158199, 0.18976, 0.206265, 0.213188, 0.219548, 0.224762, 0.22481, 0.234131, 0.256941]
Count = [0.630068, 0.628126, 0.627873, 0.627786, 0.629387, 0.626806, 0.626985, 0.626442, 0.626262, 0.625903]

data=[Comparison,Mark,BitOr,Count]
# data=np.array(data).transpose()
sums = [sum(i) for i in np.array(data).transpose()]
bottom_y = [0] * len(be)
i=0
# print(len(data),len(sums))
for d in data:
 # 计算每个柱子的高度，即百分比
 y = [a/b for a, b in zip(d, sums)]
#  print(y)
 plt.bar(be, y, width, bottom=bottom_y,label=Name[i])
 i+=1
 # 计算bottom参数的位置
 bottom_y = [(a+b) for a, b in zip(y, bottom_y)]

# plt.bar(be, BIOP5DD, width, label=Name[6],color='r',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b4, width, bottom=BIOP5DD, label=Name[5],color='y',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b3, width, bottom=BIOP2SD, label=Name[4],color='m',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b2, width, bottom=BIOP4DS, label=Name[3],color='c',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b1, width, bottom=BIOP3PD, label=Name[2],color='g',alpha=0.5,ec='k',lw=.6)
# plt.bar(be, b0, width,bottom=BIOP1SS, label=Name[1],color='b',alpha=0.5,ec='k',lw=.6) # hatch="//",
plt.yticks(np.arange(0, 1.19, 0.2))

def to_percent(temp, position):
    return '%1.0f'%(100*temp) + '%'
plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))


plt.xticks(be)
plt.xlabel('Number of Bit Exponent') # ,fontsize=13
plt.ylabel('Time Proportion (%)') # ,fontsize=13
plt.tick_params(direction='out',labelsize=12,length=4,width=1,top=False,right=False)
plt.legend(ncol=4) #fontsize=6,frameon=False,loc='upper center',ncol=6
# # ax.text(.87,-.08,'\nVisualization by DataCharm',transform = ax.transAxes,
# #         ha='center', va='center',fontsize = 5,color='black',fontweight='bold',family='Roboto Mono')


gcf = plt.gcf()
plt.show()
gcf.savefig('BIOP5DDTimeDistributionByBe.eps',format='eps', bbox_inches='tight')


