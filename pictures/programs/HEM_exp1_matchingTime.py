import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False

g = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512]
x = np.arange(10)  # x轴刻度标签位置
Name = ["Rein", "HEM", "BIOP1SS", "BIOP2SD", "BIOP3PD", "BIOP4DS", "BIOP5DD", "Simple", "Simple2"]

Rein = [7.507867, 7.523441, 7.491133, 7.469946, 7.480353, 7.478964, 7.483469, 7.497624, 7.512456, 7.548657]
HEM = [4.356678, 4.359796, 2.691707, 1.616367, 1.054942, 0.779681, 0.633634, 0.546193, 0.500432, 0.499474					
           ]
BIOP1SS = [5.304859, 3.62649, 2.239693, 1.692458, 1.400983, 1.240565, 1.163007, 1.124972, 1.100831, 1.088242
           ]
BIOP2SD = [4.460347, 2.778533, 1.966471, 1.432411, 1.176333, 1.041447, 0.956448, 0.914182, 0.891948, 0.886664
           ]
BIOP3PD = [4.273603, 4.270326, 2.604015, 1.776771, 1.369453, 1.162134, 1.05434, 1.002029, 0.974296, 0.967377]
BIOP4DS = [4.219623, 3.389834, 2.410671, 1.76837, 1.40493, 1.209748, 1.105859, 1.062175, 1.035043, 1.01118
           ]
BIOP5DD = [3.26564, 2.430373, 1.608375, 1.19609, 0.987277, 0.880073, 0.81403, 0.791269, 0.769721, 0.759325
           ]
Simple = [13.297686, 13.251958, 13.282044, 13.262612, 13.299794, 13.278775, 13.399269, 13.30123, 13.265576, 13.273719]

Memory=[157, 159, 169, 188, 226, 302, 455, 760, 1370, 2591]




fig=plt.figure()
ax = fig.add_subplot(111)

# ax.plot(be, Rein, marker='o', color='r', label=Name[0])
ax.plot(x, HEM, marker='x', color='DODGERBLUE', label=Name[1])
# ax.plot(be, BIOP1SS, marker='+', color='g', label=Name[2])
# ax.plot(be, BIOP2SD, marker='*', color='b', label=Name[3])
# ax.plot(be, BIOP3PD, marker='^', color='m', label=Name[4])
# ax.plot(be, BIOP4DS, marker='s', color='y', label=Name[5])
# ax.plot(be, BIOP5DD, marker='.', color='DEEPPINK', label=Name[6])
# ax.plot(be, Simple, marker='D',  color='lightseagreen', label=Name[7])

ax.legend(loc=(2.5/10,3.99/5),fontsize=12)
ax.grid()
ax.set_xlabel('Number of Group',fontsize=20)
ax.set_ylabel('Matching Time (ms)',fontsize=20)
# plt.xticks(range(0,10))
ax.set_xlim(-0.5,9.5)
ax.set_zorder(0)
x_major_locator=MultipleLocator(1)
ax.xaxis.set_major_locator(x_major_locator)
for size in ax.get_xticklabels():   #获取x轴上所有坐标，并设置字号
    # size.set_fontname('Times New Roman')   
    size.set_fontsize('15')

ax2 = ax.twinx()
ax2.bar(x, Memory, color='lightsteelblue',  label = 'Memory (MB)') # alpha=0.7,
ax2.set_ylabel(r"Memory Size (MB)")
ax2.set_ylim(0, 2700)
ax2.legend(loc=(5.5/10,3.99/5),fontsize=12)
ax2.set_zorder(1)
# for size in ax2.get_xticklabels():   #获取x轴上所有坐标，并设置字号
#     # size.set_fontname('Times New Roman')   
#     size.set_fontsize('16')
plt.xticks(x, labels=g)
plt.tick_params(direction='out',labelsize=13,length=4,width=1)
for a,b in zip(x,Memory):
    c=b-160
    if a<5 :
        c+=20    
    plt.text(a, c, '%.0f' % b, ha='center', va= 'bottom') #,fontsize=7

gcf = plt.gcf()
plt.show()
gcf.savefig('../matchingTime.eps',format='eps',bbox_inches='tight')


