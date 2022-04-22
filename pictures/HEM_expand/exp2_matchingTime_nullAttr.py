import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rc

rc('mathtext', default='regular')
from matplotlib.pyplot import MultipleLocator

plt.rcParams['font.family'] = ['Times New Roman']
plt.rcParams['axes.unicode_minus'] = False

be = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
gs = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
HEM0 = [34.749128, 29.549224, 24.754425, 19.391468, 13.667330, 8.295001]
HEM5_RAG = [18.832466, 16.806104, 14.874562, 8.751316, 3.763366, 0.202728]
HEM5_VAG = [18.979770, 16.921690, 14.989879, 8.676165, 3.821250, 0.772351]
Simple = [19.553517, 18.359468, 15.343778, 12.220887, 9.120504, 7.059534]
Rein = [36.452470, 41.005932, 45.921190, 51.249340, 52.458424, 56.068801]
AdaRein_ORI = [
    36.534351, 39.758585, 46.959827, 50.980289, 52.512885, 55.922359
]
HEM5_avxOR = [20.400712, 18.567820, 17.302312, 15.265979, 13.602074, 12.025513]
HEM5 = [19.088776, 17.001701, 15.213395, 12.916689, 10.679986, 8.257566]
Tama = [7.072645, 6.005393, 5.040520, 3.816577, 2.702999, 1.450566]
OpIndex2 = [61.273606, 40.896958, 25.819700, 16.657835, 8.618768, 1.331137]

# 11: 22607/1024
Name = [
    "REIN", "HEMPS", "HEMSS", "BIOP2SD", "BIOP3PD", "BIOP4DS", "HEMDD",
    "Simple", "Simple2"
]
x = np.arange(6)  # x��̶ȱ�ǩλ??
width = 0.25

plt.figure(figsize=(5, 4))

plt.bar(x - width, Rein, width, color='r',
        label=Name[0])  # r"$HEM0PS\_20S_e$" 'limegreen'
plt.bar(x, HEM0, width, color='DODGERBLUE',
        label=Name[1])  # r"$BIOP3PD\_10S_e$"
plt.bar(x + width, HEM5, width, color='DEEPPINK',
        label=Name[6])  # limegreen brown DEEPPINK r"$HEM5DD\_10S_e$"

plt.tick_params(direction='out', labelsize=15, length=5.5, width=1)
plt.xlabel('Proportion of Null Attributes', fontsize=20)
plt.ylabel('Matching Time (ms)', fontsize=20)
# plt.title('4 datasets')

plt.xticks(x, labels=["0", "0.2", "0.4", "0.6", "0.8", "0.99"])
plt.legend(fontsize=11, loc=(0.3 / 10, 2.98 / 4), ncol=1)
plt.grid()
# plt.title(r"Exp2: Marking Time when $\frac{S_e}{d}=q=0.5$",fontsize=18)

fig = plt.gcf()
plt.show()
fig.savefig('./exp2_matchingTime_nullAttr.pdf',
            format='pdf',
            bbox_inches='tight')
