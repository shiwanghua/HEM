ulimit -s unlimited
sudo chmod 777 cmakeRB
sudo chmod 777 *.txt
#./cmakeRB
expID=$1

for((bits=10;bits>=0;bits--));  #À¨ºÅ²»ÄÜÊ¡
do  
./cmakeRB $expID $bits
echo Experiment $expID \done. #$(expr $i \* 3 + 1); 
expID=`expr $expID + 1`
done
