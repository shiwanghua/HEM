#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os
import sys

expID = int(sys.argv[1])
coreId = '8'

os.system('ulimit -s unlimited;sudo chmod 777 run')
os.system('sudo chmod 777 *.txt')
os.system("rm tmpData/*.txt")
os.system("sudo cpufreq-set -c " + coreId + " -g performance")
os.system("sudo cpufreq-set -c " + coreId + " -d 3.7G")
os.system("sudo cpufreq-set -c " + coreId + " -u 4.9G")

# Exp 1: be
# need to #define DEBUG in constant.h file
# for be in range(9, -1, -1):
#     os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 20 10 20 0.0 0.4 ' + str(be)+' 1 1000000 1')
#     print('Experiment', str(expID), ': atts= 20, k= 10, m= 20, alpha= 0.0, w= 0.4, be= ', str(be), 'attrGroup= 1, valDom= 1000000, pD= 1 done.\n\n')
#     expID += 1

# be2=[1,3,6,9,12,15,18,21,24,27]
# need to set be to -1
# for be in be2:
#     os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 20 10 20 0.0 0.4 ' + str(be)+' 1 1000000 1')
#     print('Experiment', str(expID), ': atts= 20, k= 10, m= 20, alpha= 0.0, w= 0.4, be= ', str(be), 'attrGroup= 1, valDom= 1000000, pD= 1 done.\n\n')
#     expID += 1

# Exp 2: null attributes
# need to #define DEBUG in constant.h file
# need to use GenSubList_g, GenPubList_g function to generate data!
# Se = [1000, 800, 600, 400, 200, 10]
# groupNum = [1, 1, 1, 2, 5, 100]
# for i in range(6):
#     os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 1000 10 ' +
#               str(Se[i]) + ' 0.0 0.4 1 ' + str(groupNum[i]) + ' 1000000 1')
#     print(
#         'Experiment', str(expID), ': atts= 1000, k= 10, m= ' + str(Se[i]) +
#         ', alpha= 0.0, w= 0.4, be= 1, attrGroup= ' + str(groupNum[i]) +
#         ', valDom= 1000000, pD= 1 done.\n\n')
#     expID += 1

# Exp 3: vitual attribute group
# need to #define DEBUG in constant.h file
# AG = [20,50,100,400,500,1000]
# Se = [20, 20, 20, 20, 20, 20]
# for i in range(6):
#     os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 10000 5 ' + str(Se[i]) + ' 0.0 0.8 1 ' + str(AG[i])+' 1000000 1')
#     print('Experiment', str(expID),
#           ': atts= 10000, k= 5, m= ' + str(Se[i]) + ', alpha= 0.0, w= 0.8, be= 1, attrGgroup= ' + str(
#               AG[i]) + ', valDom= 1000000, pD= 1 done.\n\n')
#     expID += 1

# Exp 4: real attribute group
# need to #define DEBUG in constant.h file
# need to use GenSubList_g, GenPubList_g function to generate data!
# AG = [20,50,100,400,500,1000]
# Se = [20, 20, 20, 20, 20, 10]
# for i in range(6):
#     os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 10000 5 ' + str(Se[i]) + ' 0.0 0.8 1 ' + str(AG[i])+' 1000000 1')
#     print('Experiment', str(expID),
#           ': atts= 10000, k= 5, m= ' + str(Se[i]) + ', alpha= 0.0, w= 0.5, be= 1, attrGroup= ' + str(
#               AG[i]) + ', valDom= 1000000, pD= 1 done.\n\n')
#     expID += 1

# # Exp 5: n
# # need to open model test function.
# need to use GenPubList2 function to generate data!
# os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 20 10 20 0.0 0.4 4 1 1000000 1')
# print('Experiment', str(expID), ': atts= 20, k= 10, m= 20, alpha= 0.0, w= 0.4, be= 4, attrGroup= 1, valDom= 1000000, pD= 1 done.\n\n')
# expID += 1

# # Exp 6: Ss size of subscriptions 5-30
# need to use GenPubList2 function to generate data!
# for Ss in range(30, 0, -5):
#     os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 30 ' + str(Ss) + ' 30 0.0 0.8 4 1 1000000 1')
#     print('Experiment', str(expID), ': atts= 30, k= ' + str(Ss) + ', m= 30, alpha= 0.0, w= 0.8, be= 4, attrGroup= 1, valDom= 1000000, pD= 1 done.\n\n')
#     expID += 1

# # Exp 7: Se size of events 30-80
# for Se in range(80, 20, -10):
#     os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 80 5 ' + str(Se) + ' 0.0 0.4 4 1 1000000 1')
#     print('Experiment', str(expID), ': atts= 80, k= 5, m= ' + str(Se) + ', alpha= 0.0, w= 0.4, be= 4, attrGroup= 1, valDom= 1000000, pD= 1 done.\n\n')
#     expID += 1

# # Exp 8: width 0.1-0.7
# need to use GenPubList2 function to generate data!
# width = 0.9
# for i in range(9):
#     os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 20 5 20 0.0 ' + str(width) + ' 4 1 1000000 1')
#     print('Experiment', str(expID), ': atts= 20, k= 5, m= 20, alpha= 0.0, w= ' + str(width) + ', be= 4, attrGroup= 1, valDom= 1000000, pD= 1 done.\n\n')
#     expID += 1
#     width -= 0.1

# # Exp 9: virtual attr group
# attr = [10000, 3000, 1000, 300, 100, 30]
# be = [1, 2, 3, 4, 4, 4]
# ag = [400, 200, 100, 60, 50, 1]
# for i in range(6):
#     os.system(
#         'taskset -c ' + coreId + ' ./run ' + str(expID) + ' ' + str(attr[i]) + ' 5 20 0.0 0.8 ' + str(
#             be[i]) + ' ' + str(ag[i])+' 1000000 1')
#     print('Experiment', str(expID),
#           ': atts= ' + str(attr[i]) + ', k= 5, m= 20, alpha= 0.0, w= 0.8, be= 4, attrGroup= ' + str(
#               ag[i]) + ', valDom= 1000000, pD= 1 done.\n\n')
#     expID += 1

# # Exp 10: real attr group
# need to use GenSubList_g, GenPubList_g function to generate data!
# attr = [10000, 3000, 1000, 300, 100, 30]
# be = [1, 2, 3, 4, 4, 4]
# ag = [100, 50, 40, 15, 5, 1]
# ag = [500, 150, 50, 15, 5, 1]

#for i in range(6):
#    os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' ' +
#              str(attr[i]) + ' 5 20 0.0 0.8 ' + str(be[i]) + ' ' + str(ag[i]) +
#              ' 1000000 1')
#    print(
#        'Experiment', str(expID), ': atts= ' + str(attr[i]) +
#        ', k= 5, m= 20, alpha= 0.0, w= 0.8, be= ' + str(be[i]) +
#        ', attrGroup= ' + str(ag[i]) + ', valDom= 1000000, pD= 1 done.\n\n')
#    expID += 1

# # Exp 11: alpha 0-5 attribute distribution
# need to set attDis=1
# for alpha in range(5, -1, -1):
#     os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 100 10 20 ' + str(alpha) + ' 0.4 4 25 1000000 1')
#     print('Experiment', str(expID), ': atts= 100, k= 10, m= 20, alpha= ' + str(
#         alpha) + ', w= 0.4, be= 4, attrGroup= 25, valDom= 1000000, pD= 1 done.\n\n')
#     expID += 1

# # Exp 12: valDom
# need to use GenSubList_g, GenPubList_g function to generate data!
# for val in [1000000, 100000, 10000, 1000, 100]:
#     os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 1000 10 20 0.0 0.8 1 40 ' + str(val)+ ' 1')
#     print('Experiment', str(expID),
#           ': atts= 1000, k= 10, m= 20, alpha= 0.0, w= 0.8, be= 1, attrGroup= 40, valDom= ' + str(val) + ', pD= 1 done.\n\n')
#     expID += 1
# need to set buks=100
# os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 1000 10 20 0.0 0.8 1 40 100 1')
# print('Experiment', str(expID),': atts= 1000, k= 10, m= 20, alpha= 0.0, w= 0.8, be= 1, attrGroup= 40, valDom= 100, pD= 1 done.\n\n')

# # Exp 13: comprehensive
# need to set attNumType=1, attDis=1, valDis=1
# need to #define DEBUG in constant.h file
# os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 100 10 40 1 0.0 4 25 1000000 1')
# print('Experiment', str(expID),
#       ': atts= 100, k= 10, m= 20, alpha= 1, w= 0.0, be= 4, attrGroup= 25, valDom= 1000000, pD= 1 done.\n\n')
# expID += 1

# # Exp 14: insert/delete
# need to use GenPubList2 function to generate data!
# need to set verifyID to true
# for Ss in range(30, 0, -5):
#     os.system('taskset -c ' + coreId + ' ./run ' + str(expID) + ' 30 ' + str(Ss) + ' 30 0.0 0.8 4 1 1000000 1')
#     print('Experiment', str(expID), ': atts= 30, k= ' + str(Ss) + ', m= 30, alpha= 0.0, w= 0.8, be= 4, attrGroup= 1, valDom= 1000000, pD= 1 done.\n\n')
#     expID += 1

# Exp 16: SIMD
# dimen=[16, 32, 64, 128, 256, 512] # =\psi_e
# for d in dimen:
#     os.system(' ./run ' + str(expID) + ' '+str(d)+' 10 '+str(d)+' 0.0 0.4 4 1 1000000 1')
#     print('Experiment', str(expID), ': atts= '+str(d)+', k= 10, m= '+str(d)+', alpha= 0.0, w= 0.4, be= 4, attrGroup= 1, valDom= 1000000, pD= 1 done.\n\n')
#     expID += 1

# Exp 17: parallel degree 
# Exp 18: pHEMDD-avxOR 组合实验
# for pD in [1,2,4,8,16,32,64]:
#     os.system(' ./run ' + str(expID) + ' 512 10 512 0.0 0.4 4 1 1000000 ' + str(pD))
#     print('Experiment', str(expID), ': atts= 512, k= 10, m= 512, alpha= 0.0, w= 0.4, be= 4, attrGroup= 1, valDom= 1000000, pD= ' + str(pD) + ' done.\n\n')
#     expID += 1

# Exp 19: pHEMDD-RAS-avxOR
# please set n=134217728
for pD in [1,2,4,8,16,32,64]:
    os.system(' ./run ' + str(expID) + ' 2048 10 64 0.0 0.4 1 32 1000000 ' + str(pD))
    print('Experiment', str(expID), ': atts= 2048, k= 10, m= 64, alpha= 0.0, w= 0.4, be= 1, attrGroup= 32, valDom= 1000000, pD= ' + str(pD) + ' done.\n\n')
    expID += 1

path = "tmpData"
content = ""
for fileName in os.listdir(path):
    file_path = os.path.join(path, fileName)
    f = open(file_path, 'r')
    content += fileName[:-4] + " = [" + f.read()[:-2] + "]\n"
print(content)