#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os
import sys
os.system('ulimit -s unlimited;sudo chmod 777 cmakeRB')
os.system('sudo chmod 777 *.txt')
expID=int(sys.argv[1])

# # Exp 1 2
# for be in range(9,-1,-1):
#     os.system('taskset -c 18 ./cmakeRB '+str(expID)+' 20 '+str(be)+' 0.3')
#     print('Experiment',str(expID),' atts=m=',str(20),' be= ',str(be),' w=0.3 done.\n\n')
#     expID+=1


# # Ss size of subscriptions 5-30
# for Ss in range(30,0,-5):
#     os.system('taskset -c 18 ./cmakeRB '+str(expID)+' '+str(Ss)+' 5 0.7')
#     print('Experiment',str(expID),' Ss=',str(Ss),' be= 5, w=0.7 done.\n\n')
#     expID+=1

# # Se size of events 30-80
# for Se in range(80,20,-10):
#     os.system('taskset -c 18 ./cmakeRB '+str(expID)+' '+str(Se)+' 5 0.3')
#     print('Experiment ',str(expID),' Se=',str(30),' be= 5, w=0.3 done.\n\n')
#     expID+=1

# # width 0.1-0.7
# width=0.9
# for i in range(9):
#     os.system('taskset -c 18 ./cmakeRB '+str(expID)+' 20 5 '+str(width))
#     print('Experiment',str(expID),' atts=m=',str(20),' be=',str(5),'width=',width,'done.\n\n')
#     expID+=1
#     width-=0.1

# atts 30-900
for atts in [900,700,500,300,100,30]:
   os.system('taskset -c 18 ./cmakeRB '+str(expID)+' '+ str(atts)+ ' 5 0.5')
   print('Experiment',str(expID),': atts=',str(atts), ', m=20, be=5, w=0.5 done.\n\n')
   expID+=1

# # alpha 0-5 attribute distribution
# for alpha in range(5,-1,-1):
#    os.system('taskset -c 18 ./cmakeRB '+str(expID)+' '+ str(alpha)+ ' 5 0.3')
#    print('Experiment',str(expID),': alpha=',str(alpha), ', atts=50, m=20, cons=10, be=5, w=0.3 done.\n\n')
#    expID+=1

path="tmpData"
content=""
for fileName in os.listdir(path):
    file_path = os.path.join(path, fileName)
    f=open(file_path,'r')
    content+=fileName[:-4]+" = ["+f.read()[:-2]+"]\n"
print(content)
os.system("rm tmpData/*.txt")