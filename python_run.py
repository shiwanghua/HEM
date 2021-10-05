import os
import sys
os.system('ulimit -s unlimited;sudo chmod 777 cmakeRB')
os.system('sudo chmod 777 *.txt')
expID=int(sys.argv[1])

#for m in range(10000,0,-1000):
#    os.system('./cmakeRB '+str(expID)+' '+str(m))
#    print('Experiment',str(expID),' ','atts=m= ',str(m),'done.')
#    expID+=1

for be in range(9,-1,-1):
    os.system('taskset -c 0 ./cmakeRB '+str(expID)+' 20 '+str(be)+' 0.3')
    print('Experiment',str(expID),' atts=m=',str(20),' be= ',str(be),' w=0.3 done.')
    expID+=1

#for m in range(10000,0,-1000):
#    os.system('./cmakeRB '+str(expID)+' '+ str(m)+ ' 0 0.3')
#    print('Experiment',str(expID),' atts=m=',str(m),' be=',str(5),'done.')
#    expID+=1

'''
width=0.9
for i in range(9):
    os.system('./cmakeRB '+str(expID)+' 20 5 '+str(width))
    print('Experiment',str(expID),' atts=m=',str(20),' be=',str(5),'width=',str(width),'done.')
    expID+=1
    width-=0.1
'''
