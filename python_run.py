import os
import sys
os.system('ulimit -s unlimited;sudo chmod 777 cmakeRB')
os.system('sudo chmod 777 *.txt')
expID=int(sys.argv[1])

for m in range(1000,0,-100):
    os.system('./cmakeRB '+str(expID)+' '+str(m))
    print('Experiment',str(expID),' ','m= ',str(m),'done.')
    expID+=1

