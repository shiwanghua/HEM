import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc
from matplotlib.pyplot import MultipleLocator

rc('mathtext', default='regular')

plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['font.family'] = ['Times New Roman']  #
Name = ["REIN", "HEM", "Simple", "TAMA", "Ada-REIN", "OpIndex"]
x = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]

# HEM0 = [0.467408, 0.531190, 0.525600, 0.491829, 0.465316, 0.432144, 0.486478, 0.461491, 0.437459]
HEM5_RAG = [
    0.103364, 0.151319, 0.183362, 0.212182, 0.225473, 0.235648, 0.267188,
    0.302204, 0.351841
]
HEM5_VAG = [
    0.096444, 0.149327, 0.180394, 0.209312, 0.230389, 0.234096, 0.259749,
    0.298672, 0.345959
]
Simple = [
    11.947397, 14.267041, 14.769717, 14.430894, 13.937573, 13.185469,
    12.764541, 11.047522, 9.123215
]
Rein = [
    1.774599, 2.732667, 2.697823, 3.086133, 3.168597, 3.765589, 4.748034,
    5.345962, 5.804990
]
AdaREIN = [
    1.784836, 2.739094, 2.540308, 2.796412, 3.228154, 3.758323, 4.679871,
    5.240862, 5.891610
]
HEM = [
    0.102709, 0.150727, 0.182855, 0.215658, 0.225488, 0.238247, 0.261228,
    0.301697, 0.347519
]
TAMA = [
    4.410415, 4.615393, 3.786352, 3.311397, 3.033121, 2.967110, 2.722695,
    2.662048, 2.107400
]
OpIndex = [
    10.991445, 11.662716, 11.225464, 11.248130, 11.192717, 11.147342,
    11.188172, 11.356302, 11.228356
]
Rein.reverse()
AdaREIN.reverse()
HEM.reverse()
TAMA.reverse()
OpIndex.reverse()

lsize = 24

fig = plt.figure(figsize=(5, 4))
ax = fig.add_subplot(111)
ax.set_xlabel('Value of Predicate Widths', fontsize=lsize)
ax.set_ylabel('Matching Time (ms)', fontsize=lsize)
# plt.xticks(range(0,10))
ax.plot(x, Rein, marker='v', color='r', label=Name[0])
ax.plot(x, HEM, marker='.', color='DODGERBLUE', label=Name[1])
# ax.plot(x, Simple, marker='D', color='deepskyblue', label=Name[2]) #
ax.plot(x, TAMA, marker='*', color='DarkCyan', label=Name[3])
ax.plot(x, AdaREIN, marker='x', color='DarkMagenta', label=Name[4])
ax.plot(x, OpIndex, marker='h', color='DimGray', label=Name[5])  #   slategray

ax.legend(fontsize=13, ncol=2, loc=(1.65 / 5, 2.3 / 5),
          columnspacing=0.3)  #fontsize=10    loc='upper left'
ax.grid()
ax.set_xlim(0.1, 0.9)
ax.set_xticks(x)
# ax.set_xticklabels(x)
# ax.set_yscale("log")
ax.set_ylim(0, 12.5)
# ax.set_yticks([0,3,6,9,12,15])
# ax.set_yticklabels(['-1', '0', '1'])
ax.set_zorder(0)
plt.tick_params(labelsize=24)
gcf = plt.gcf()
plt.show()
gcf.savefig('./exp8_w.pdf', format='pdf', bbox_inches='tight')
