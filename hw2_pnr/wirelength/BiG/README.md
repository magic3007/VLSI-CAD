# BiG

bivariate gradient-based wirelength operator

## problem

- forward: calculate BiG wirelength with pin position
- backward: calculate gradient backward on pin position

formulation: see [BiG](https://ieeexplore.ieee.org/abstract/document/8806973)

## build

```
mkdir build
cd build 
cmake .. 
make
```

## requirements 

- complete the code in ``big_wa.cpp``
- do NOT modify other files when handed in
- the program will randomly generate pin positions for test
- some test cases are in ``./dataset''
