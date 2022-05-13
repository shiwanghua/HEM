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
pHEMDD_RAS_S = [
    2.60798968237536, 3.68774407174557, 6.10425653547217, 7.2290451431817,
    6.46422882512347, 2.87629467513913, 1.72703189457009
]

x = np.arange(7)
width = 0.4
lsize = 24
tickSize = 20

plt.figure(figsize=(5, 4))
# plt.tight_layout()

# plt.bar(x - width, Rein, width, color='r',
# label=Name[0])  # r"$HEM0PS\_20S_e$" 'limegreen'
plt.bar(x, pHEMDD_RAS_S, width,
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
plt.yticks([0, 2, 4, 6, 8])

fig = plt.gcf()
plt.show()
fig.savefig('./exp19_pHEM_RAS_avxOR.pdf', format='pdf', bbox_inches='tight')
