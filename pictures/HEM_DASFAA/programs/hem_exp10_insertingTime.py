import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman'] # 
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [5,10,15,20,25,30]

# Construction
Rein = [0.000176, 0.000268, 0.000355, 0.000519, 0.000642, 0.000732]
HEM = [0.00028, 0.000418, 0.000515, 0.000677, 0.000847, 0.000974]
Simple = [0.000049, 0.00005, 0.000053, 0.000051, 0.000063, 0.000061]
TAMA = [0.00182, 0.003612, 0.005383, 0.007094, 0.008819, 0.010593]
AdaREIN = [0.000178, 0.000256, 0.000352, 0.000491, 0.000617, 0.000723]
OpIndex = [0.000082, 0.000124, 0.00017, 0.000212, 0.00029, 0.00034]	

mul=1000 # ms
Rein=np.array(Rein)*mul
HEM=np.array(HEM)*mul
Simple=np.array(Simple)*mul
TAMA=np.array(TAMA)*mul
AdaREIN=np.array(AdaREIN)*mul
OpIndex=np.array(OpIndex)*mul

lsize=24

fig=plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Size of Subscriptions', fontsize=lsize)
ax.set_ylabel('Inserting Time (us)', fontsize=lsize,labelpad=0)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend(loc=(1.62/5,2.75/5), ncol=2,fontsize=12)# 
ax.grid()
ax.set_xlim(5,30)
# ax.set_xticks([0,1,2,3,4,5])
ax.set_xticklabels(x)
ax.set_yscale("log",base=10,subs=[2,3,4,5,6,7,8,9])
ax.set_ylim(0,12)
# ax.set_yticks([0,2,8,32,128,256])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=18)
gcf = plt.gcf()
plt.show()
gcf.savefig('../exp10_construction.eps',format='eps',bbox_inches='tight')