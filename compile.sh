g++ -Wall -Wall -std=c++11 -m64 -fPIC -shared -fvisibility=hidden -Wl,--as-needed -O2  -c again.cpp -o vst3_plugin.o
g++ -shared  vst3_plugin.o  -o vst3_plugin.vst3/Contents/x86_64-linux/vst3_plugin.so -shared -m64 -s -s  -lrt

