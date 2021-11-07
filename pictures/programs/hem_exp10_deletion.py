import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
# from matplotlib.pyplot import MultipleLocator
from matplotlib.ticker import MultipleLocator, FormatStrFormatter
rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman'] # 
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [5,10,15,20,25,30]


# Deletion
Rein = [0.007082, 0.021233, 0.038501, 0.059884, 0.084521, 0.115474					
			
]
HEM = [0.009854, 0.024151, 0.042893, 0.065137, 0.09292, 0.126659					
				
]
Simple = [1.277313, 1.2029, 1.212332, 1.20232, 1.216849, 1.287885					
				
]
TAMA = [1.089468, 3.160207, 6.113018, 10.157548, 15.019955, 21.002387					
					
]
AdaREIN = [0.006419, 0.02022, 0.036488, 0.057858, 0.08205, 0.112875					
				
]
OpIndex = [0.966205, 2.344291, 3.850941, 6.13071, 9.138021, 7.7087									
]	

mul=1 # ms
Rein=np.array(Rein)*mul
HEM=np.array(HEM)*mul
Simple=np.array(Simple)*mul
TAMA=np.array(TAMA)*mul
AdaREIN=np.array(AdaREIN)*mul
OpIndex=np.array(OpIndex)*mul

fig=plt.figure(figsize=(5, 4))
lsize=24
ax = fig.add_subplot(111)
ax.set_xlabel('Size of Subscriptions', fontsize=lsize)
ax.set_ylabel('Deleting Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5]) #   slategray

ax.legend( fontsize=12, ncol=2) #fontsize=10 loc=(1.36/5,0.01/5),
ax.grid()
ax.set_xlim(5,30)
# ax.set_xticks([0,1,2,3,4,5])
ax.set_xticklabels(x)
# ax.set_yscale("log")
ax.set_yscale("log",base=10,subs=[2,3,4,5,6,7,8,9])
ax.set_ylim(0,24)
# ax.set_yticks([0,2,4,8,16])
# ax.set_yticklabels([0,2,4,8,16])
ax.set_zorder(0)

# xmajorLocator   = MultipleLocator(1)
# ax.xaxis.set_major_locator(xmajorLocator)
# xminorLocator   = MultipleLocator(0.25)
# ax.xaxis.set_minor_locator(xminorLocator)

# ymajorLocator   = MultipleLocator(2)
# ax.yaxis.set_major_locator(ymajorLocator)
# yminorLocator   = MultipleLocator(1)
# ax.yaxis.set_minor_locator(yminorLocator)
plt.tick_params(labelsize=18)
gcf = plt.gcf()
plt.show()
gcf.savefig('../exp10_deletion.eps',format='eps',bbox_inches='tight')