import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc

rc('mathtext', default='regular')
from matplotlib.pyplot import MultipleLocator

plt.rcParams['font.family'] = ['Times New Roman']
plt.rcParams['axes.unicode_minus'] = False

pD = [2, 4, 8, 16, 32, 64]
pHEM5_HEM5=[1.502, 3.198, 5.929, 11.248, 10.31, 6.241 ]
per_pHEM5_HEM5= [0.751, 0.8, 0.741, 0.703, 0.322, 0.098 ]

x = np.arange(6)  
width = 0.25
lsize = 24

plt.figure(figsize=(8, 4))

plt.subplot(121)

# plt.bar(x - width, Rein, width, color='r',
        # label=Name[0])  # r"$HEM0PS\_20S_e$" 'limegreen'
plt.bar(x, pHEM5_HEM5, width, color='DODGERBLUE',
        )  # label=Name[1] r"$BIOP3PD\_10S_e$"
# plt.bar(x + width, HEM5, width, color='DEEPPINK',
#         label=Name[6])  # limegreen brown DEEPPINK r"$HEM5DD\_10S_e$"

plt.tick_params(labelsize=18)
# plt.tick_params(direction='out', labelsize=24, length=5.5, width=1)
plt.xlabel('Number of Thread', fontsize=lsize)
plt.ylabel('SpeedUp', fontsize=lsize)

plt.xticks(x, labels=["2", "4", "8", "16", "32", "64"])
# plt.legend(fontsize=11, loc=(0.3 / 10, 2.98 / 4), ncol=1)
plt.grid()
# plt.title(r"Exp2: Marking Time when $\frac{S_e}{d}=q=0.5$",fontsize=18)

plt.subplot(122)
plt.plot(x, per_pHEM5_HEM5, marker='h', color='DimGray') #, label=Name[5]

fig = plt.gcf()
plt.show()
fig.savefig('./exp17_pHEM.pdf',
            format='pdf',
            bbox_inches='tight')
