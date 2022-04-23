import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman']  #
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = ["30", "100", "300", "1k", "3k", "10k"]

HEM5_VAS = [7.295973, 3.713577, 2.102555, 0.966343, 0.402103, 0.182992]

HEM5_RAS = [0.839040, 0.458072, 0.166370, 0.110122, 0.114420, 0.195089]
Simple = [18.770278, 11.935546, 6.644946, 6.175464, 7.642632, 15.301071]
Rein = [51.430875, 33.421445, 20.910710, 10.842777, 5.906751, 2.699236]
AdaRein_ORI = [48.727632, 31.732389, 19.661532, 10.387039, 5.706645, 2.648183]
HEM5_avxOR = [121.703362, 36.601154, 11.917077, 2.881154, 0.631253, 0.207212]
HEM5 = [91.304853, 25.569989, 8.305902, 2.318742, 0.616655, 0.193013]
Tama = [1.376154, 1.481647, 1.547340, 1.856587, 2.510774, 3.153018]
OpIndex2 = [1.012507, 1.170977, 1.409074, 2.314469, 3.936489, 5.782785]

Rein.reverse()
Simple.reverse()
AdaRein_ORI.reverse()
HEM5_RAS.reverse()
Tama.reverse()
OpIndex2.reverse()

lsize = 24
fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Number of Dimensions', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM5_RAS, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, Tama, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaRein_ORI, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex2, marker='h', color='DimGray', label=Name[5])  #   slategray

ax.legend(fontsize=11, ncol=2, loc=(0.1 / 5, 0.8 / 5))  #'lower right'
ax.grid()
ax.set_xlim(0, 5)
ax.set_xticks([0, 1, 2, 3, 4, 5])
ax.set_xticklabels(x)
ax.set_yscale("log", base=4, subs=[2, 3])
# ax.set_ylim(0, 60)
ax.set_yticks([0.25, 1, 4, 16, 64])
ax.set_yticklabels(['0.25', '1', '4', '16', '64'])

# ax.set_yticks([0,2,8,32,128,256])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=24)
gcf = plt.gcf()
plt.show()
gcf.savefig('./exp10_d_RAS.pdf', format='pdf', bbox_inches='tight')
