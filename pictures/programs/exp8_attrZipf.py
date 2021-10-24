import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False

Name = ["REIN", "BIOP5DD", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [0,1,2,3,4,5]

Rein = [4.791532, 5.145358, 6.061476, 7.230236, 8.840719, 10.058959]
BIOP5DD = [0.993307, 1.019171, 0.999629, 0.996959, 0.972336, 0.961571]
Simple = [12.240371, 12.622173, 13.569352, 14.46068, 13.681989, 9.805888]
TAMA = [3.985104, 4.255728, 4.03668, 3.907889, 3.496185, 3.141536]
AdaREIN = [3.024179, 3.159921, 3.033639, 3.453758, 4.215174, 6.695819]
OpIndex = [17.173711, 15.133199, 11.047338, 6.688679, 4.946488, 5.714342]	

fig=plt.figure()
ax = fig.add_subplot(111)
ax.set_xlabel('Value of Alpha')
ax.set_ylabel('Matching Time (ms)')
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, BIOP5DD, marker='.', color='DEEPPINK', label=Name[1])
ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend( ncol=3) #fontsize=10 loc=(1.36/5,0.01/5),
ax.grid()
ax.set_xlim(0,5)
ax.set_xticks([0,1,2,3,4,5])
ax.set_xticklabels(x)
# ax.set_yscale("log")
# ax.set_yticks([0,2,8,32,128,256])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)

gcf = plt.gcf()
plt.show()
gcf.savefig('../exp8_alpha.eps',format='eps',bbox_inches='tight')