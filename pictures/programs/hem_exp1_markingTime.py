import matplotlib
import matplotlib.pyplot as plt
import numpy as np
# 解决坐标轴负号问题
plt.rcParams['axes.unicode_minus'] = False

be = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512]
Rein = [6.748365, 6.75273, 6.740895, 6.764119, 6.759414, 6.752019, 6.851964, 6.906145, 6.794111, 6.787625]
HEM = [3.981146, 3.977086, 2.268488, 1.163715, 0.601853, 0.311117, 0.163021, 0.077129, 0.035397, 0.012259]
# BIOP1SS = [4.349099, 2.71147, 1.296991, 0.722027, 0.426176, 0.269556, 0.185458, 0.134792, 0.099483, 0.062853]
# BIOP2SD = [3.726824, 2.130125, 1.253618, 0.67853, 0.395699, 0.244584, 0.157231, 0.113065, 0.078423, 0.0505]
# BIOP3PD = [3.453845, 3.450873, 1.740693, 0.866495, 0.44478, 0.221289, 0.110529, 0.056167, 0.025463, 0.010273]
# BIOP4DS = [3.296822, 2.489885, 1.498227, 0.844949, 0.473075, 0.272294, 0.17711, 0.125551, 0.082788, 0.035308]
# BIOP5DD = [2.608424, 1.817173, 0.957682, 0.515203, 0.303935, 0.187489, 0.12812, 0.092957, 0.065522, 0.029597]
# Simple = [13.297686, 13.251958, 13.282044, 13.262612, 13.299794, 13.278775, 13.399269, 13.30123, 13.265576, 13.273719]

Name = ["Rein", "HEM", "BIOP1SS", "BIOP2SD", "BIOP3PD", "BIOP4DS", "BIOP5DD", "Simple", "Simple2"]

plt.figure(figsize=(5, 4))
# plt.subplot()
x = np.arange(10)  # x轴刻度标签位置
width = 0.5  # 柱子的宽度
# 计算每个柱子在x轴上的位置，保证x轴刻度标签居中
plt.bar(x, HEM,width,  color='DODGERBLUE', label=Name[1])
# plt.bar(x - 1.5*width, BIOP1SS, width, color='purple', label=Name[2])
# plt.bar(x - 0.5*width, BIOP2SD, width, color='darkorange', label=Name[3])
# plt.bar(x + 0.5*width, BIOP3PD, width, color='limegreen', label=Name[4])
# plt.bar(x + 1.5*width, BIOP4DS, width, color='brown', label=Name[5])
# plt.bar(x + 2.5*width, BIOP5DD, width, color='DEEPPINK', label=Name[6])
plt.tick_params(direction='out',labelsize=8,length=4,width=1)
plt.xlabel('Value of Bit Exponent',fontsize=10)
plt.ylabel('Marking Time (ms)',fontsize=10)
# plt.title('4 datasets')
# x轴刻度标签位置不进行计算
plt.xticks(x, labels=be)
plt.legend(fontsize=10,ncol=2)
plt.grid()
fig = plt.gcf()
plt.show()
fig.savefig('../bemarking.eps',format='eps',bbox_inches='tight')