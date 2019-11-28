# nash - Never Alive SHell

## Features
* Piping
* Conditional execution (&&, ||)
* Variables
* Subshell expansion

## Builtins
* exit
* cd
* set \[var\] \[val\]

## Build Steps

### Debug Build
```
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=debug ..
make -j$(nproc)
```

### Release Build
```
mkdir build
cd build
cmake ..
make -j$(nproc)
```
