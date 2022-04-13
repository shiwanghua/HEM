import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman']  #
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [0, 1, 2, 3, 4, 5]

HEM0 = [1.373163, 1.370359, 1.371137, 1.597900, 1.181744, 1.179349]
HEM5_RAG = [0.458043, 0.456186, 0.452017, 0.447066, 0.388313, 0.347644]
HEM5_VAG = [0.600182, 0.605624, 0.619425, 0.723300, 0.640186, 0.640873]
Simple = [12.026263, 12.650959, 12.861924, 14.284002, 12.567761, 8.734940]
Rein = [5.867533, 6.133382, 6.858576, 9.467056, 15.315819, 15.583356]
AdaRein_ORI = [5.672621, 5.879912, 6.658780, 9.406161, 13.411411, 16.330703]
HEM5_avxOR = [0.984743, 0.983160, 0.977838, 1.011617, 0.914694, 0.891080]
HEM5 = [0.923545, 0.917288, 0.918413, 1.006460, 0.855803, 0.811353]
Tama = [3.845069, 3.974011, 3.838606, 4.117466, 3.373556, 2.309741]
OpIndex2 = [18.949450, 15.942777, 10.795097, 5.741489, 2.737105, 2.693930]

Rein.reverse()
Simple.reverse()
AdaRein_ORI.reverse()
HEM5.reverse()
Tama.reverse()
OpIndex2.reverse()

lsize = 24
fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel(r'Value of $\alpha$', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM5, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, Tama, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaRein_ORI, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex2, marker='h', color='DimGray', label=Name[5])  #   slategray

ax.legend(fontsize=12, loc="upper left", ncol=3)  #fontsize=10
ax.grid()
ax.set_xlim(0, 5)
ax.set_xticks([0, 1, 2, 3, 4, 5])
ax.set_xticklabels(x)
# ax.set_yscale("log")
# ax.set_yticks([0,2,8,32,128,256])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=22)
gcf = plt.gcf()
plt.show()
gcf.savefig('./exp11_alpha.pdf', format='pdf', bbox_inches='tight')
