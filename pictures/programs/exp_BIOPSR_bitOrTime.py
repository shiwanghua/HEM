import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
rc('mathtext', default='regular')
from matplotlib.pyplot import MultipleLocator
plt.rcParams['axes.unicode_minus'] = False

be = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
gs = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
Rein = [6.817679, 6.830059, 6.81814, 6.837105, 6.818885,
        6.82175, 6.822016, 6.841621, 6.806594, 6.854903]
BIOP3PD_20Se = [3.453845, 3.450873, 1.740693, 0.866495,
                0.44478, 0.221289, 0.110529, 0.056167, 0.025463, 0.010273]
BIOP3PD_10Se = [3.757521, 1.963476, 0.979669, 0.501525,
                0.251967, 0.130293, 0.065142, 0.031459, 0.014392, 0.005791]
BIOP5DD_10Se = [3.227029, 1.022528, 0.526503, 0.290197,
                0.165956, 0.103507, 0.068803, 0.04792, 0.033578, 0.014289]

BIOPSR_orTime = [300,270,240,210,180,150,120,90,60,30]
Memory = [390/1024,629/1024,708/1024,867/1024,1153/1024,1657/1024,2572/1024,4205/1024,7226/1024,12597/1024]
# 11: 22607/1024
Name = ["Rein", "BIOP0PS", "BIOP1SS", "BIOP2SD",
        "BIOP3PD", "BIOP4DS", "BIOP5DD", "Simple", "Simple2"]
x = np.arange(10)  
width = 0.25  


fig=plt.figure(figsize=(9, 9))

ax=fig.add_subplot(111)
ax.set_xlabel('Group Size', fontsize=20)
ax.set_ylabel('Bit Or Operation Time (ms)', fontsize=20)
# plt.xlabel('Group Size', fontsize=20)
# plt.ylabel('Bit Or Operation Time (ms)', fontsize=20)
# plt.xticks(x, labels=gs)

ax.plot(gs, BIOPSR_orTime, marker='x', color='teal', label="BIOPSR")
ax.set_xlim(0.1,10.9)
ax.set_zorder(0)
ax.legend(fontsize=20, loc=(4.54/10,4.4/5))
ax.grid()
x_major_locator=MultipleLocator(1)
ax.xaxis.set_major_locator(x_major_locator)

ax2 = ax.twinx()
ax2.bar(gs, Memory, color='lightsteelblue',  label = 'Memory (GB)') # alpha=0.7,
ax2.set_ylabel(r"Memory Size (GB)", fontsize=20)
ax2.set_ylim(0, 12.5)
ax2.legend(fontsize=20, loc=(4.54/10,3.94/5))
ax2.set_zorder(1)
# ax2.set_title(r"Exp3: Bit Or Operation Time and Memory of BIOPSR by $g$",fontsize=18)


fig = plt.gcf()
plt.show()
fig.savefig('../exp3_bitOrTime.eps', format='eps', bbox_inches='tight')
