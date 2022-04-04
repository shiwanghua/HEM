import matplotlib
import matplotlib.pyplot as plt
import numpy as np
# 解决坐标轴负号问题
plt.rcParams['axes.unicode_minus'] = False

# Rein = [14.141, 14.139, 14.136, 14.145, 14.126, 14.148, 14.133, 14.126, 14.142, 14.163 ]
# BIOP0PS = [8.624, 8.642, 5.047, 2.670, 1.431, 0.789, 0.427, 0.218, 0.097, 0.034 ]
# BIOP1SS = [10.450, 7.391, 4.213, 2.924, 2.162, 1.669, 1.362, 1.147, 0.928, 0.620]
# BIOP2SD = [8.626, 5.701, 3.838 2.559, 1.818, 1.354, 1.061, 0.853, 0.678, 0.450 ]
# BIOP3PD = [7.250, 7.253, 3.766, 1.963, 1.034, 0.552, 0.297, 0.151, 0.069, 0.030 ]
# BIOP4DS = [8.083, 6.709, 4.695, 3.242, 2.378, 1.777, 1.428, 1.136, 0.883, 0.405 ]
# BIOP5DD = [6.306, 5.008, 3.109, 2.086, 1.542, 1.152, 0.924, 0.764, 0.571, 0.243 ]
# Simple = [83.854363, 83.730956, 83.71685, 83.732012, 83.704068, 83.699345, 83.738354, 84.104656, 83.754113, 83.969767]
be = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
Rein = [6.817679, 6.830059, 6.81814, 6.837105, 6.818885, 6.82175, 6.822016, 6.841621, 6.806594, 6.854903]
BIOP0PS = [4.013783, 4.018383, 2.324436, 1.193692, 0.624261, 0.318925, 0.161674, 0.079613, 0.036091, 0.012815]
BIOP1SS = [4.349099, 2.71147, 1.296991, 0.722027, 0.426176, 0.269556, 0.185458, 0.134792, 0.099483, 0.062853]
BIOP2SD = [3.726824, 2.130125, 1.253618, 0.67853, 0.395699, 0.244584, 0.157231, 0.113065, 0.078423, 0.0505]
BIOP3PD = [3.453845, 3.450873, 1.740693, 0.866495, 0.44478, 0.221289, 0.110529, 0.056167, 0.025463, 0.010273]
BIOP4DS = [3.296822, 2.489885, 1.498227, 0.844949, 0.473075, 0.272294, 0.17711, 0.125551, 0.082788, 0.035308]
BIOP5DD = [2.608424, 1.817173, 0.957682, 0.515203, 0.303935, 0.187489, 0.12812, 0.092957, 0.065522, 0.029597]
Simple = [13.297686, 13.251958, 13.282044, 13.262612, 13.299794, 13.278775, 13.399269, 13.30123, 13.265576, 13.273719]

Name = ["Rein", "BIOP0PS", "BIOP1SS", "BIOP2SD", "BIOP3PD", "BIOP4DS", "BIOP5DD", "Simple", "Simple2"]

plt.figure(figsize=(20, 8))
# plt.subplot()
x = np.arange(10)  # x轴刻度标签位置
width = 0.14  # 柱子的宽度
# 计算每个柱子在x轴上的位置，保证x轴刻度标签居中
plt.bar(x - 2.5*width, BIOP0PS, width, color='DODGERBLUE', label=Name[1])
plt.bar(x - 1.5*width, BIOP1SS, width, color='purple', label=Name[2])
plt.bar(x - 0.5*width, BIOP2SD, width, color='darkorange', label=Name[3])
plt.bar(x + 0.5*width, BIOP3PD, width, color='limegreen', label=Name[4])
plt.bar(x + 1.5*width, BIOP4DS, width, color='brown', label=Name[5])
plt.bar(x + 2.5*width, BIOP5DD, width, color='DEEPPINK', label=Name[6])
plt.tick_params(direction='out',labelsize=15,length=5.5,width=1)
plt.xlabel('Value of Bit Exponent',fontsize=20)
plt.ylabel('Marking Time (ms)',fontsize=20)
# plt.title('4 datasets')
# x轴刻度标签位置不进行计算
plt.xticks(x, labels=be)
plt.legend(fontsize=20,ncol=2)
plt.grid()
fig = plt.gcf()
plt.show()
fig.savefig('../bemarking.eps',format='eps',bbox_inches='tight')

# plt.figure()
# # plt.plot(be, Rein, marker='o', color='r', label=Name[0])
# plt.plot(be, BIOP0PS, marker='x', color='b', label=Name[1])
# plt.plot(be, BIOP1SS, marker='+', color='g', label=Name[2])
# plt.plot(be, BIOP2SD, marker='*', color='c', label=Name[3])
# plt.plot(be, BIOP3PD, marker='^', color='m', label=Name[4])
# plt.plot(be, BIOP4DS, marker='s', color='y', label=Name[5])
# plt.plot(be, BIOP5DD, marker='.', color='k', label=Name[6])
# # plt.plot(be, Simple, marker='D', color='aliceblue', label=Name[7])
# plt.legend()
# plt.xlabel('Bit Exponent')
# plt.ylabel('Marking Time (ms)')
# plt.xticks(range(0,10))
# fig = plt.gcf()
# plt.show()
# fig.savefig('bemarking.png')


# b0 = [a - b for a, b in zip(BIOP0PS, BIOP1SS)]
# b1 = [a - b for a, b in zip(BIOP1SS, BIOP3PD)]
# b2= [a - b for a, b in zip(BIOP2SD, BIOP4DS)]
# b3= [a - b for a, b in zip(BIOP3PD, BIOP2SD)]
# b4= [a - b for a, b in zip(BIOP4DS, BIOP5DD)]
#
#
# width = 0.3       # the width of the bars: can also be len(x) sequence
# # fig, ax = plt.subplots(figsize=(10,3),dpi=200)
#
# plt.bar(be, BIOP5DD, width, label=Name[6],color='r',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b4, width, bottom=BIOP5DD, label=Name[5],color='y',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b3, width, bottom=BIOP2SD, label=Name[4],color='m',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b2, width, bottom=BIOP4DS, label=Name[3],color='c',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b1, width, bottom=BIOP3PD, label=Name[2],color='g',alpha=0.5,ec='k',lw=.6)
# plt.bar(be, b0, width,bottom=BIOP1SS, label=Name[1],color='b',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.yticks(np.arange(0, 5, 0.5))
# plt.xticks(be)
# plt.xlabel('Bit Exponent',fontsize=13)
# plt.ylabel('Marking Time (ms)',fontsize=13)
# # ax.set_title('Scores by group and gender')
# plt.tick_params(direction='out',labelsize=12,length=5.5,width=1,top=False,right=False)
# plt.legend() #fontsize=6,frameon=False,loc='upper center',ncol=6
# # ax.text(.87,-.08,'\nVisualization by DataCharm',transform = ax.transAxes,
# #         ha='center', va='center',fontsize = 5,color='black',fontweight='bold',family='Roboto Mono')
# plt.savefig(r'bemarking2.png', dpi=500,bbox_inches='tight')
# plt.show()

