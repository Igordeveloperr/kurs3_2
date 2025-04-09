g++ -O2 -g -fopenmp myOmp.cpp -o myOmp -I/usr/local/pvm3/include -L/usr/local/pvm3/lib/LINUX64 -lpvm3 -ltirpc
g++ -O2 -g myPosix.cpp -o myPosix -I/usr/local/pvm3/include -L/usr/local/pvm3/lib/LINUX64 -lpvm3 -ltirpc
mpic++ -O2 mpiRealiz.cpp -o mpiRealiz
g++ -O2 -g myMpi.cpp -o myMpi -I/usr/local/pvm3/include -L/usr/local/pvm3/lib/LINUX64 -lpvm3 -ltirpc
g++ -O2 -g myPvm.cpp -o myPvm -I/usr/local/pvm3/include -L/usr/local/pvm3/lib/LINUX64 -lpvm3 -ltirpc
g++ -O2 -g line.cpp -o line -I/usr/local/pvm3/include -L/usr/local/pvm3/lib/LINUX64 -lpvm3 -ltirpc
g++ -O2 -g FFTworker2.cpp -o FFTworker2 -I/usr/local/pvm3/include -L/usr/local/pvm3/lib/LINUX64 -lpvm3 -ltirpc
g++ -O2 -g FFTworker1.cpp -o FFTworker1 -I/usr/local/pvm3/include -L/usr/local/pvm3/lib/LINUX64 -lpvm3 -ltirpc
g++ -O2 -g main.cpp -o main -I/usr/local/pvm3/include -L/usr/local/pvm3/lib/LINUX64 -lpvm3 -ltirpc