# 2-way partitioning

## problem

- 2-way FM algorithm 

## build

```
mkdir build
cd build 
cmake .. 
make
```

## requirements

- complete the function ``Partitioner::Run``
- remember to have a look at members ``blockBelong`` and ``blockNodeNum``
- do NOT make any change to ``Partitioner::print_result``
- you can compile and run with ``./partition -h xxx.hgr`` to test on ISPD98 benchmark
- cost is determined by:
  + cross-partition penalty
  + the difference between the numbers of nodes in two partitions
