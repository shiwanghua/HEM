import matplotlib
import matplotlib.pyplot as plt
import numpy as np
# 解决坐标轴负号问题
plt.rcParams['axes.unicode_minus'] = False

be = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

HEM4DS = [2.776140, 1.685165, 1.070355, 0.722923, 0.501190, 0.367319, 0.316891, 0.281798, 0.263125, 0.250238]
HEM0PS = [6.273495, 4.781779, 2.171592, 1.035566, 0.648708, 0.445792, 0.368903, 0.324813, 0.440076, 0.302247]
HEM5_RAG = [2.268200, 1.303265, 0.714360, 0.446621, 0.320168, 0.256228, 0.235016, 0.219805, 0.209685, 0.195970]
HEM5_VAG = [2.172268, 1.403725, 0.672404, 0.431309, 0.308924, 0.251452, 0.225921, 0.214653, 0.210679, 0.191146]
HEMSR = [4.736042, 5.735895, 4.612845, 4.643836, 4.620032, 4.539652, 4.608578, 4.621926, 4.632537, 4.620033]
Rein = [7.002336, 6.595961, 7.351229, 6.606781, 6.651962, 6.715950, 6.603381, 6.582430, 6.619547, 6.609429]
HEM5_avxOR = [2.225551, 1.253821, 0.679921, 0.435322, 0.323962, 0.266535, 0.232815, 0.220896, 0.210343, 0.204633]
HEM3PD = [6.162609, 2.558513, 1.162424, 0.626552, 0.406727, 0.270322, 0.220610, 0.192333, 0.247660, 0.221509]
HEMSC = [1.532988, 0.844481, 0.593137, 0.449465, 0.389169, 0.343743, 0.327409, 0.318438, 0.316031, 0.305171]
HEM5DD = [2.202456, 1.174252, 0.672344, 0.431968, 0.311317, 0.249285, 0.223943, 0.214652, 0.205108, 0.193234]
HEM1SS = [2.914707, 2.405431, 1.173535, 0.747683, 0.539255, 0.450987, 0.429358, 0.378858, 0.367352, 0.386273]
HEM2SD = [2.194664, 1.789330, 1.001863, 0.655345, 0.478635, 0.390273, 0.351716, 0.334473, 0.327827, 0.318934]


Name = ["Rein", "HEM0PS", "HEM1SS", "HEM2SD", "HEM3PD", "HEM4DS", "HEM5DD"]

plt.figure(figsize=(20, 8))
# plt.subplot()
x = np.arange(10)  # x轴刻度标签位置
width = 0.14  # 柱子的宽度
# 计算每个柱子在x轴上的位置，保证x轴刻度标签居中
plt.bar(x - 2.5*width, HEM0PS, width, color='DODGERBLUE', label=Name[1])
plt.bar(x - 1.5*width, HEM1SS, width, color='purple', label=Name[2])
plt.bar(x - 0.5*width, HEM2SD, width, color='darkorange', label=Name[3])
plt.bar(x + 0.5*width, HEM3PD, width, color='limegreen', label=Name[4])
plt.bar(x + 1.5*width, HEM4DS, width, color='brown', label=Name[5])
plt.bar(x + 2.5*width, HEM5DD, width, color='DEEPPINK', label=Name[6])
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
# plt.plot(be, HEM0PS, marker='x', color='b', label=Name[1])
# plt.plot(be, HEM1SS, marker='+', color='g', label=Name[2])
# plt.plot(be, HEM2SD, marker='*', color='c', label=Name[3])
# plt.plot(be, HEM3PD, marker='^', color='m', label=Name[4])
# plt.plot(be, HEM4DS, marker='s', color='y', label=Name[5])
# plt.plot(be, HEM5DD, marker='.', color='k', label=Name[6])
# # plt.plot(be, Simple, marker='D', color='aliceblue', label=Name[7])
# plt.legend()
# plt.xlabel('Bit Exponent')
# plt.ylabel('Marking Time (ms)')
# plt.xticks(range(0,10))
# fig = plt.gcf()
# plt.show()
# fig.savefig('bemarking.png')


# b0 = [a - b for a, b in zip(HEM0PS, HEM1SS)]
# b1 = [a - b for a, b in zip(HEM1SS, HEM3PD)]
# b2= [a - b for a, b in zip(HEM2SD, HEM4DS)]
# b3= [a - b for a, b in zip(HEM3PD, HEM2SD)]
# b4= [a - b for a, b in zip(HEM4DS, HEM5DD)]
#
#
# width = 0.3       # the width of the bars: can also be len(x) sequence
# # fig, ax = plt.subplots(figsize=(10,3),dpi=200)
#
# plt.bar(be, HEM5DD, width, label=Name[6],color='r',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b4, width, bottom=HEM5DD, label=Name[5],color='y',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b3, width, bottom=HEM2SD, label=Name[4],color='m',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b2, width, bottom=HEM4DS, label=Name[3],color='c',alpha=0.5,ec='k',lw=.6) # hatch="//",
# plt.bar(be, b1, width, bottom=HEM3PD, label=Name[2],color='g',alpha=0.5,ec='k',lw=.6)
# plt.bar(be, b0, width,bottom=HEM1SS, label=Name[1],color='b',alpha=0.5,ec='k',lw=.6) # hatch="//",
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

