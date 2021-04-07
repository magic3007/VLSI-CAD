# VLSI-CAD
🍪  Implementation for the lesson Chip Design Automation and Intelligent Optimization(2021 Spring, advised by Prof. Yibo Lin) in Peking University.

## Getting Started

[comment]: <> (### Prerequisites)

### Installation

```bash
git submodule update
mkdir build && cd build
cmake ..
make
```

### Running & Test
- Run benchmarks with validation
```bash
make run_autology
make run_equivalence
make run_techmap 1> /dev/null
```
- Run with unittest.
```bash
ctest -R autology
ctest -R equivalence
ctest -R techmap
```