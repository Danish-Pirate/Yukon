# Quick Start Guide

1. Clone the repository
Pull the main repository and include submodules
```
git clone --recurse-submodules git@github.com:Danish-Pirate/Yukon.git
cd Yukon
```
2. Configure
```
mkdir -p build
cmake -B ./build
```
3. Build Yukon Core
```
cmake --build ./build --target yukon_core --parallel 14
```
4. Build Yukon GUI
```
cmake --build ./build --target yukon_gui --parallel 14
```
# Executables and usage
- Execute the Yukon core executable
```
./build/yukon_core
```
- Execute the Yukon GUI executable
```
./build/yukon_gui
```


