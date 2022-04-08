import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc

rc('mathtext', default='regular')
from matplotlib.pyplot import MultipleLocator
# ��������Ḻ����??
plt.rcParams['axes.unicode_minus'] = False

be = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
gs = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
Rein = [
    6.817679, 6.830059, 6.81814, 6.837105, 6.818885, 6.82175, 6.822016,
    6.841621, 6.806594, 6.854903
]
BIOP3PD_20Se = [
    3.453845, 3.450873, 1.740693, 0.866495, 0.44478, 0.221289, 0.110529,
    0.056167, 0.025463, 0.010273
]
BIOP3PD_10Se = [
    3.757521, 1.963476, 0.979669, 0.501525, 0.251967, 0.130293, 0.065142,
    0.031459, 0.014392, 0.005791
]
BIOP5DD_10Se = [
    3.227029, 1.022528, 0.526503, 0.290197, 0.165956, 0.103507, 0.068803,
    0.04792, 0.033578, 0.014289
]

# 11: 22607/1024
Name = [
    "Rein", "BIOP0PS", "BIOP1SS", "BIOP2SD", "BIOP3PD", "BIOP4DS", "BIOP5DD",
    "Simple", "Simple2"
]
x = np.arange(10)  # x��̶ȱ�ǩλ??
width = 0.25  # ���ӵ�???��

plt.figure(figsize=(9, 9))

# ����ÿ��������x���ϵ�λ??����֤x��̶ȱ�ǩ��??
plt.bar(x - width,
        BIOP3PD_20Se,
        width,
        color='limegreen',
        label=r"$BIOP3PD\_20S_e$")
plt.bar(x, BIOP3PD_10Se, width, color='darkorchid', label=r"$BIOP3PD\_10S_e$")
plt.bar(x + width,
        BIOP5DD_10Se,
        width,
        color='DEEPPINK',
        label=r"$BIOP5DD\_10S_e$")  # limegreen brown DEEPPINK

plt.tick_params(direction='out', labelsize=15, length=5.5, width=1)
plt.xlabel('Number of Bit Exponent', fontsize=20)
plt.ylabel('Marking Time (ms)', fontsize=20)
# plt.title('4 datasets')
# x��̶ȱ�ǩλ??������???��
plt.xticks(x, labels=be)
plt.legend(fontsize=20, loc=(5.48 / 10, 3.05 / 4))
plt.grid()
# plt.title(r"Exp2: Marking Time when $\frac{S_e}{d}=q=0.5$",fontsize=18)

fig = plt.gcf()
plt.show()
fig.savefig('./exp2_matchingTime_nullAttr.pdf',
            format='eps',
            bbox_inches='tight')
