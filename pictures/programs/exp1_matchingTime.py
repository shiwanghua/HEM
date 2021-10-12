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

Rein = [7.493299, 7.50581, 7.493681, 7.512909, 7.494312, 7.497563, 7.497439, 7.516959, 7.482396, 7.528121]
BIOP0PS = [4.67221, 4.677301, 3.076813, 1.997971, 1.457131, 1.170899, 1.015575, 0.935686, 0.894014, 0.872285]
BIOP1SS = [5.27616, 3.624523, 2.250369, 1.698706, 1.415267, 1.26773, 1.18964, 1.136168, 1.109691, 1.095123]
BIOP2SD = [4.416072, 2.799921, 1.978173, 1.448345, 1.189174, 1.056968, 0.973968, 0.930432, 0.900897, 0.888323]
BIOP3PD = [4.054831, 4.051791, 2.41175, 1.572339, 1.172832, 0.958122, 0.849146, 0.797178, 0.768841, 0.752189]
BIOP4DS = [4.196224, 3.382389, 2.423762, 1.787394, 1.420887, 1.220413, 1.126285, 1.077802, 1.042773, 1.019621]
BIOP5DD = [3.465601, 2.658464, 1.829583, 1.402715, 1.200012, 1.086948, 1.034108, 0.998188, 0.980359, 0.966726]
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


