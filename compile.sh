g++ -std=c++11 *.h *.cpp -o linuxRB  -O3
echo "Compilation finishes."
ulimit -s unlimited
chmod 777 linuxRB
./linuxRB
