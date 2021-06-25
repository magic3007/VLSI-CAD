# Static timing analysis given delay values

Forward and backward propagation

## problem

- Given delay values in the timing graph
- Propagate delays and compute slacks 

## build

```
mkdir build
cd build 
cmake .. 
make
```

## requirements

- complete the code in ``./src/timer-exercise.cpp``
- some naive cases in ``./benchmarks`` with answers
- usage: ``./timer <delay file> <timing input file>``
