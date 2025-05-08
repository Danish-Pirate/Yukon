This repository contains a Yukon game written for the 02322 Machine oriented programming course at Danish Technical University

The [TUI](https://github.com/Danish-Pirate/Yukon/tree/TUI) branch contains a Terminal user interface version of the game, and the [Master](https://github.com/Danish-Pirate/Yukon/tree/master) branch contains a GUI version of the same made using SDL.

# Quick Start Guide
## 1. Clone the repository
Pull the main repository and include submodules
```
git clone --recurse-submodules git@github.com:Danish-Pirate/Yukon.git
cd Yukon
```
## 2. Configure
```
mkdir -p build
cmake -B ./build
```
## 3. Build Yukon Core
```
cmake --build ./build --target yukon_core --parallel 14
```
## 4. Build Yukon GUI
```
cmake --build ./build --target yukon_gui --parallel 14
```
# Executables and usage
## - Execute the Yukon core executable
```
./build/yukon_core
```
## - Execute the Yukon GUI executable
```
./build/yukon_gui
```

# Note
- The GUI will only work properly with a resolution of 1920x1080
