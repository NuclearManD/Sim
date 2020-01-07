
mkdir -p ike/out/
cd ike/out/
gcc -c ../src/*.c -I../include -O3
cd ../..

mkdir -p cemuarch/out/
cd cemuarch/out/
gcc -c ../src/*.c -I../include -O3
cd ../..

mkdir -p build
cd build
g++ -c ../src/*.cpp -I../include -I../ike/include -I../cemuarch/include -O3
g++ *.o ../ike/out/*.o ../cemuarch/out/*.o -lSDL2main -lSDL2 -lSDL2_image -lm -pthread -O3 -o sim
cd ..
mkdir -p bin
mv build/sim bin/
