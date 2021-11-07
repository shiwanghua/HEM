import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.ticker import FuncFormatter 
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')
plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman'] # 
# plt.rcParams.update({'font.size': 8})
plt.figure(figsize=(5, 4))
be = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512]
width = 0.35
Name = ["Comparison", "Mark", "BitOR", "Check"]

Comparison = [0.04433, 0.044523, 0.044509, 0.044696, 0.044827, 0.045133, 0.047053, 0.046604, 0.047575, 0.047257]
Mark = [3.981146, 3.977086, 2.268488, 1.163715, 0.601853, 0.311117, 0.163021, 0.077129, 0.035397, 0.012259]
BitOr = [0.188111, 0.189921, 0.278124, 0.32792, 0.350478, 0.373873, 0.388627, 0.383803, 0.393144, 0.389137]
Count = [0.032936, 0.03299, 0.033007, 0.032981, 0.032992, 0.033021, 0.033261, 0.033101, 0.03309, 0.032851]

data=[Comparison,Mark,BitOr,Count]
x = np.arange(10)
# data=np.array(data).transpose()
sums = [sum(i) for i in np.array(data).transpose()]
bottom_y = [0] * len(be)
i=0
# print(len(data),len(sums))
Color=['pink','DODGERBLUE','lightblue','r']
for d in data:
 y = [a/b for a, b in zip(d, sums)]
#  print(y)
 plt.bar(x, y, width, bottom=bottom_y,color=Color[i],label=Name[i])
 i+=1
 bottom_y = [(a+b) for a, b in zip(y, bottom_y)]

# plt.bar(be, BIOP5DD, width, label=Name[6],color='r',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b4, width, bottom=BIOP5DD, label=Name[5],color='y',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b3, width, bottom=BIOP2SD, label=Name[4],color='m',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b2, width, bottom=BIOP4DS, label=Name[3],color='c',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b1, width, bottom=BIOP3PD, label=Name[2],color='g',alpha=0.5,ec='k',lw=.6)
# plt.bar(be, b0, width,bottom=BIOP1SS, label=Name[1],color='b',alpha=0.5,ec='k',lw=.6) # hatch="//",

def to_percent(temp, position):
    return '%1.0f'%(100*temp) + '%'
plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))


plt.xticks(x, labels=be)
plt.yticks(np.arange(0, 1.01, 0.1))
plt.xlabel('Number of Groups',fontsize=20) # ,fontsize=13
plt.ylabel('Time Proportion (%)',fontsize=20) # ,fontsize=13
plt.tick_params(direction='out',labelsize=13,length=4,width=1,top=False,right=False)
plt.legend(fontsize=9, loc=(-0.014,1.01),ncol=4) #fontsize=6,frameon=False,loc='upper center',ncol=6
# # ax.text(.87,-.08,'\nVisualization by DataCharm',transform = ax.transAxes,
# #         ha='center', va='center',fontsize = 5,color='black',fontweight='bold',family='Roboto Mono')


gcf = plt.gcf()
plt.show()
gcf.savefig('../BIOP0PSTimeDistributionByBe.eps',format='eps', bbox_inches='tight')


