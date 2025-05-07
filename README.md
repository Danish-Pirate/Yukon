# Quick Start Guide

1. Clone the repository
Pull the main repository and include submodules
```
git clone --recurse-submodules git@github.com:Danish-Pirate/Yukon.git
cd Yukon
```
2. Build and compile in release mode
```
mkdir -p cmake-build-release
cmake -B ./cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build ./cmake-build-release --target yukon_gui --config Release --parallel 14
```
