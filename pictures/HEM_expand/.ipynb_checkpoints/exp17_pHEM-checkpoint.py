import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.ticker import FuncFormatter

rc('mathtext', default='regular')
from matplotlib.pyplot import MultipleLocator

plt.rcParams['font.family'] = ['Times New Roman']
plt.rcParams['axes.unicode_minus'] = False

pD = [2, 4, 8, 16, 32, 64]

# pHEM5_HEM5 = [1.502, 3.198, 5.929, 11.248, 10.31, 6.241]
# per_pHEM5_HEM5 = [0.751, 0.8, 0.741, 0.703, 0.322, 0.098]

pHEM5_HEM5=[1.534, 3.003, 5.71, 12.146, 15.632, 14.154 ]
per_pHEM5_HEM5= [0.767, 0.75075, 0.71375, 0.759125, 0.4885, 0.22115625 ]

x = np.arange(6)
width = 0.4
lsize = 24 # 34
tickSize = 20 # 30


# plt.figure(figsize=(18, 6))
# # plt.tight_layout()

# plt.subplots_adjust(left=None, bottom=None, right=None, top=None, \
#     wspace=0.25, hspace=0.45)

# plt.subplot(121)
# # plt.bar(x - width, Rein, width, color='r',
# # label=Name[0])  # r"$HEM0PS\_20S_e$" 'limegreen'
# plt.bar(x, pHEM5_HEM5, width,
#         color='#00BFFF')  # label=Name[1] r"$BIOP3PD\_10S_e$"
# # plt.bar(x + width, HEM5, width, color='DEEPPINK',
# #         label=Name[6])  # limegreen brown DEEPPINK r"$HEM5DD\_10S_e$"

# plt.tick_params(labelsize=tickSize)
# # plt.tick_params(direction='out', labelsize=24, length=5.5, width=1)
# plt.xlabel('Parallel Degree', fontsize=lsize)
# plt.ylabel('SpeedUp', fontsize=lsize)

# plt.xticks(x, labels=["2", "4", "8", "16", "32", "64"])
# # plt.legend(fontsize=11, loc=(0.3 / 10, 2.98 / 4), ncol=1)
# plt.grid()
# # plt.title(r"Exp2: Marking Time when $\frac{S_e}{d}=q=0.5$",fontsize=18)
# plt.yticks([0, 3, 6, 9, 12])

# plt.subplot(122)
# plt.bar(x, per_pHEM5_HEM5, width, color='#00BFFF')  #, label=Name[5]
# plt.xlabel('Parallel Degree', fontsize=lsize)
# plt.ylabel('Efficiency (%)', fontsize=lsize)
# plt.grid()
# plt.xticks(x, labels=["2", "4", "8", "16", "32", "64"])
# plt.tick_params(labelsize=tickSize)
# def to_percent(temp, position):
#     return '%1.0f' % (100 * temp) + '%'

# plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))
# fig = plt.gcf()
# plt.show()
# fig.savefig('./exp17_pHEM.pdf', format='pdf', bbox_inches='tight')

plt.figure(figsize=(5, 4))
# plt.tight_layout()

# plt.subplots_adjust(left=None, bottom=None, right=None, top=None, \
    # wspace=0.25, hspace=0.45)

# plt.subplot(121)
# plt.bar(x - width, Rein, width, color='r',
# label=Name[0])  # r"$HEM0PS\_20S_e$" 'limegreen'
plt.bar(x, pHEM5_HEM5, width, color='#00BFFF')  # label=Name[1] r"$BIOP3PD\_10S_e$"
# plt.bar(x + width, HEM5, width, color='DEEPPINK',
#         label=Name[6])  # limegreen brown DEEPPINK r"$HEM5DD\_10S_e$"

plt.tick_params(labelsize=tickSize)
# plt.tick_params(direction='out', labelsize=24, length=5.5, width=1)
plt.xlabel('Parallel Degree', fontsize=lsize)
plt.ylabel('SpeedUp', fontsize=lsize)

plt.xticks(x, labels=["2", "4", "8", "16", "32", "64"])
# plt.legend(fontsize=11, loc=(0.3 / 10, 2.98 / 4), ncol=1)
plt.grid()
# plt.title(r"Exp2: Marking Time when $\frac{S_e}{d}=q=0.5$",fontsize=18)
plt.yticks([0, 3, 6, 9, 12])

fig = plt.gcf()
plt.show()
fig.savefig('./exp17_pHEM_speedup.pdf', format='pdf', bbox_inches='tight')

plt.figure(figsize=(5, 4))
plt.bar(x, per_pHEM5_HEM5, width, color='#00BFFF')  #, label=Name[5]
plt.xlabel('Parallel Degree', fontsize=lsize)
plt.ylabel('Efficiency (%)', fontsize=lsize)
plt.grid()
plt.xticks(x, labels=["2", "4", "8", "16", "32", "64"])
plt.tick_params(labelsize=tickSize)
def to_percent(temp, position):
    return '%1.0f' % (100 * temp) + '%'

plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))
fig = plt.gcf()
plt.show()
fig.savefig('./exp17_pHEM_efficieny.pdf', format='pdf', bbox_inches='tight')
