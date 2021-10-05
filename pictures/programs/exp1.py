from matplotlib import pyplot as plt 

be = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
Rein = [14.141, 14.139, 14.136, 14.145, 14.126, 14.148, 14.133, 14.126, 14.142, 14.163 ]
BIOP0PS = [8.624, 8.642, 5.047, 2.670, 1.431, 0.789, 0.427, 0.218, 0.097, 0.034 ]
BIOP1SS = [10.450, 7.391, 4.213, 2.924, 2.162, 1.669, 1.362, 1.147, 0.928, 0.620]
BIOP2SD = [8.626, 5.701, 3.838, 2.559, 1.818, 1.354, 1.061, 0.853, 0.678, 0.450 ]
BIOP3PD = [7.250, 7.253, 3.766, 1.963, 1.034, 0.552, 0.297, 0.151, 0.069, 0.030 ]
BIOP4DS = [8.083, 6.709, 4.695, 3.242, 2.378, 1.777, 1.428, 1.136, 0.883, 0.405 ]
BIOP5DD = [6.306, 5.008, 3.109, 2.086, 1.542, 1.152, 0.924, 0.764, 0.571, 0.243 ]
Simple = [83.854363, 83.730956, 83.71685, 83.732012, 83.704068, 83.699345, 83.738354, 84.104656, 83.754113, 83.969767]

Name = ["Rein", "BIOPPS", "BIOPSS", "BIOPSD", "BIOPPD", "BIOPDS", "BIOPDD", "Simple", "Simple2"]
plt.figure()
# plt.plot(be, Rein, marker='o', color='r', label=Name[0])
plt.plot(be, BIOP0PS, marker='x', color='b', label=Name[1])
plt.plot(be, BIOP1SS, marker='+', color='g', label=Name[2])
plt.plot(be, BIOP2SD, marker='*', color='c', label=Name[3])
plt.plot(be, BIOP3PD, marker='^', color='m', label=Name[4])
plt.plot(be, BIOP4DS, marker='s', color='y', label=Name[5])
plt.plot(be, BIOP5DD, marker='.', color='k', label=Name[6])
# plt.plot(be, Simple, marker='D', color='aliceblue', label=Name[7])
plt.legend()
plt.xlabel('Bit Exponent')
plt.ylabel('Marking Time (ms)')
fig = plt.gcf()
plt.show()
fig.savefig('bemarking.pdf')
