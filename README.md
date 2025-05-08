This repository contains a Yukon game written for the 02322 Machine oriented programming course at Danish Technical University

The [TUI](https://github.com/Danish-Pirate/Yukon/tree/TUI) branch contains a Terminal user interface version of the game, and the [Master](https://github.com/Danish-Pirate/Yukon/tree/master) branch contains a GUI version of the same made using SDL.

# Quick Start Guide
## 1. Clone the repository
Pull the main repository and include submodules
```
git clone -b TUI git@github.com:Danish-Pirate/Yukon.git
cd Yukon
```
## 2. Configure
```
mkdir -p build
cmake -B ./build
```
## 3. Build game
```
cmake --build ./build
```
## 4. Execute the Yukon core executable
```
./build/Yukon_Cardgame
```


