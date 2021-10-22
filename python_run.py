import os
import sys
os.system('ulimit -s unlimited;sudo chmod 777 cmakeRB')
os.system('sudo chmod 777 *.txt')
expID=int(sys.argv[1])

# for m in range(20,9,-2):
#    os.system('taskset -c 20 ./cmakeRB '+str(expID)+m+' 4 0.3')
#    print('Experiment',str(expID),' ','atts=20, m= ',str(m),'done.')
#    expID+=1

# for be in range(9,-1,-1):
#     os.system('taskset -c 20 ./cmakeRB '+str(expID)+' 10 '+str(be)+' 0.3')
#     print('Experiment',str(expID),' atts=m=',str(20),' be= ',str(be),' w=0.3 done.')
#     expID+=1

#for m in range(10000,0,-1000):
#    os.system('./cmakeRB '+str(expID)+' '+ str(m)+ ' 0 0.3')
#    print('Experiment',str(expID),' atts=m=',str(m),' be=',str(5),'done.')
#    expID+=1

# width 0.1-0.7
width=0.7
for i in range(7):
    os.system('taskset -c 20 ./cmakeRB '+str(expID)+' 20 4 '+str(width))
    print('Experiment',str(expID),' atts=m=',str(20),' be=',str(4),'width=',width,'done.')
    expID+=1
    width-=0.1


# # Ss size of subscriptions 5-30
# for Ss in range(30,0,-5):
#     os.system('taskset -c 20 ./cmakeRB '+str(expID)+' '+str(Ss)+' 4 0.7')
#     print('Experiment',str(expID),' Ss=',str(30),' be= 4, w=0.3 done.')
#     expID+=1

# # Se size of events 30-80
# for Se in range(80,20,-10):
#     os.system('taskset -c 20 ./cmakeRB '+str(expID)+' '+str(Se)+' 4 0.3')
#     print('Experiment ',str(expID),' Se=',str(30),' be= 4, w=0.3 done.')
#     expID+=1