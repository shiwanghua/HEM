import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')
# 解决坐标轴负号?
plt.rcParams['axes.unicode_minus'] = False

be = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
Name = ["Rein", "BIOP0PS", "BIOP1SS", "BIOP2SD", "BIOP3PD", "BIOP4DS", "BIOP5DD", "Simple", "Simple2"]

Rein = [7.507867, 7.523441, 7.491133, 7.469946, 7.480353, 7.478964, 7.483469, 7.497624, 7.512456, 7.548657]
BIOP0PS = [4.67888, 4.683258, 3.063765, 1.984039, 1.441388, 1.15816, 1.000263, 0.921542, 0.886562, 0.877483
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


# time = np.arange(10)
# temp = np.random.random(10)*30
# Swdown = np.random.random(10)*100-10
# Rn = np.random.random(10)*100-10
#
# fig = plt.figure()
# ax = fig.add_subplot(111)
# ax.plot(time, Swdown, '-', label = 'Swdown')
# ax.plot(time, Rn, '-', label = 'Rn')
# ax2 = ax.twinx()
# ax2.plot(time, temp, '-r', label = 'temp')
# ax.legend(loc=0)
# ax.grid()
# ax.set_xlabel("Time (h)")
# ax.set_ylabel(r"Radiation ($MJ\,m^{-2}\,d^{-1}$)")
# ax2.set_ylabel(r"Temperature ($^\circ$C)")
# ax2.set_ylim(0, 35)
# ax.set_ylim(-20,100)
# ax2.legend(loc=0)


fig=plt.figure()
ax = fig.add_subplot(111)

# ax.plot(be, Rein, marker='o', color='r', label=Name[0])
ax.plot(be, BIOP0PS, marker='x', color='DODGERBLUE', label=Name[1])
# ax.plot(be, BIOP1SS, marker='+', color='g', label=Name[2])
# ax.plot(be, BIOP2SD, marker='*', color='b', label=Name[3])
# ax.plot(be, BIOP3PD, marker='^', color='m', label=Name[4])
# ax.plot(be, BIOP4DS, marker='s', color='y', label=Name[5])
ax.plot(be, BIOP5DD, marker='.', color='DEEPPINK', label=Name[6])
# ax.plot(be, Simple, marker='D',  color='lightseagreen', label=Name[7])

ax.legend(loc=(2/10,3.95/5))
ax.grid()
ax.set_xlabel('Bit Exponent')
ax.set_ylabel('Matching Time (ms)')
# plt.xticks(range(0,10))
ax.set_xlim(-0.5,9.5)
ax.set_zorder(0)
x_major_locator=MultipleLocator(1)
ax.xaxis.set_major_locator(x_major_locator)

ax2 = ax.twinx()
ax2.bar(be, Memory, color='lightsteelblue',  label = 'Memory (MB)') # alpha=0.7,
ax2.set_ylabel(r"Memory Size (MB)")
ax2.set_ylim(0, 2700)
ax2.legend(loc=(5.1/10,3.95/5))
ax2.set_zorder(0)

for a,b in zip(be,Memory):
    c=b-160
    if a<5 :
        c+=20    
    plt.text(a, c, '%.0f' % b, ha='center', va= 'bottom') #,fontsize=7

gcf = plt.gcf()
plt.show()
gcf.savefig('matchingTime.eps',format='eps',bbox_inches='tight')


