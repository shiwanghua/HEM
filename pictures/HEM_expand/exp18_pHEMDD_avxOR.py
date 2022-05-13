import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.ticker import FuncFormatter

rc('mathtext', default='regular')
from matplotlib.pyplot import MultipleLocator

plt.rcParams['font.family'] = ['Times New Roman']
plt.rcParams['axes.unicode_minus'] = False

pD = [1, 2, 4, 8, 16, 32, 64]
pHEM5_avxOR_HEM5 = [1.677, 2.289, 4.832, 9.698, 13.974, 13.278, 7.937]

x = np.arange(7)
width = 0.4
lsize = 24
tickSize = 20

plt.figure(figsize=(5, 4))
# plt.tight_layout()

# plt.bar(x - width, Rein, width, color='r',
# label=Name[0])  # r"$HEM0PS\_20S_e$" 'limegreen'
plt.bar(x, pHEM5_avxOR_HEM5, width,
        color='#00BFFF')  # label=Name[1] r"$BIOP3PD\_10S_e$"
# plt.bar(x + width, HEM5, width, color='DEEPPINK',
#         label=Name[6])  # limegreen brown DEEPPINK r"$HEM5DD\_10S_e$"

plt.tick_params(labelsize=tickSize)
# plt.tick_params(direction='out', labelsize=24, length=5.5, width=1)
plt.xlabel('Parallel Degree', fontsize=lsize)
plt.ylabel('SpeedUp', fontsize=lsize)

plt.xticks(x, labels=["1", "2", "4", "8", "16", "32", "64"])
# plt.legend(fontsize=11, loc=(0.3 / 10, 2.98 / 4), ncol=1)
plt.grid()
# plt.title(r"Exp2: Marking Time when $\frac{S_e}{d}=q=0.5$",fontsize=18)
plt.yticks([0, 3, 6, 9, 12, 15])

fig = plt.gcf()
plt.show()
fig.savefig('./exp18_pHEMDD_avxOR.pdf', format='pdf', bbox_inches='tight')
