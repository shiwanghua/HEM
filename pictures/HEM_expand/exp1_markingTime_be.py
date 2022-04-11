import matplotlib
import matplotlib.pyplot as plt
import numpy as np
# 解决坐标轴负号问题
plt.rcParams['axes.unicode_minus'] = False

HEM4DS = [
    4.047486, 2.526655, 1.552167, 0.794416, 0.430514, 0.245464, 0.163093,
    0.113702, 0.066706, 0.039161
]
HEM0PS = [
    6.179008, 3.92577, 2.010077, 1.148819, 0.556959, 0.287136, 0.145834,
    0.070261, 0.031981, 0.011284
]

HEM3PD = [
    6.289745, 2.767193, 1.401119, 0.680176, 0.342275, 0.175087, 0.089294,
    0.043542, 0.020153, 0.007893
]

HEM5DD = [
    2.815691, 1.387544, 0.728088, 0.391327, 0.229163, 0.143344, 0.09613,
    0.068335, 0.049382, 0.015175
]
HEM1SS = [
    4.614529, 2.54399, 1.267462, 0.687892, 0.385353, 0.241119, 0.162618,
    0.120626, 0.088552, 0.056948
]
HEM2SD = [
    3.328289, 1.994791, 1.098463, 0.603167, 0.342783, 0.201764, 0.130021,
    0.088015, 0.061921, 0.039159
]

Memory=[157, 159, 169, 188, 226, 302, 455, 760, 1370, 2591]


Name = ["Rein", "HEM0PS", "HEM1SS", "HEM2SD", "HEM3PD", "HEM4DS", "HEM5DD"]

plt.figure(figsize=(14, 6))
# plt.subplot()
x = np.arange(10)  # x轴刻度标签位置
be = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512]
width = 0.14  # 柱子的宽度
# 计算每个柱子在x轴上的位置，保证x轴刻度标签居中
plt.bar(x - 2.5 * width, HEM0PS, width, color='DODGERBLUE', label=Name[1])
plt.bar(x - 1.5 * width, HEM1SS, width, color='purple', label=Name[2])
plt.bar(x - 0.5 * width, HEM2SD, width, color='darkorange', label=Name[3])
plt.bar(x + 0.5 * width, HEM3PD, width, color='limegreen', label=Name[4])
plt.bar(x + 1.5 * width, HEM4DS, width, color='brown', label=Name[5])
plt.bar(x + 2.5 * width, HEM5DD, width, color='DEEPPINK', label=Name[6])
plt.tick_params(direction='out', labelsize=15, length=5.5, width=1)
plt.xlabel('Number of Groups', fontsize=20)
plt.ylabel('Marking Time (ms)', fontsize=20)
# plt.title('4 datasets')
# x轴刻度标签位置不进行计算
plt.xticks(x, labels=be)
plt.legend(fontsize=20, ncol=2)
plt.grid()
fig = plt.gcf()
plt.show()
fig.savefig('./exp1_markingTime_be.pdf', format='pdf', bbox_inches='tight')

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
