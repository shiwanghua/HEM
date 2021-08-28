ulimit -s unlimited
sudo chmod 777 cmakeRB
#./cmakeRB
$expID=$1

for((bits=0;bits<=7;bits++));  #À¨ºÅ²»ÄÜÊ¡
do  
expID=`expr $expID + 1`
sudo ./cmakeRB $expID $bits
echo Experiment $expID \done. #$(expr $i \* 3 + 1); 
done
