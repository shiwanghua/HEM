sudo g++ -std=c++11 *.h *.cpp -o linuxRB  -O3
echo "Compilation finishes."
ulimit -s unlimited
sudo chmod 777 linuxRB

./linuxRB 136 7

for
