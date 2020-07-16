# Die2Sim
Die2Sim is a tool used to aid simulations of large scale superconducting circuits. The main usage of Die2Sim is to create JoSIM files from a DEF file using an existing cell library.

Version: 1.0

## Main Features
* Convert DEF to JoSIM.
* Create flow diagram of the circuit. (Experimental)

## Getting Started

### Prerequisites

The following packages is required to successfully compile and execute Die2Sim.

``` bash
apt install build-essencials cmake  # for compiling
apt install graphviz                # for creating hierarchy diagram(dot file)
```

### Installation

``` bash
# Current directory: Die2Sim root
mkdir build && cd build
cmake ..
make
```

### Usage

Examples of how to execute Die2Sim:

#### DEF to JoSIM

``` bash
./Die2Sim -j defExample.def -o josimOutput.cir
```

#### Help

``` bash
./Die2Sim -h
```

## Configuration
In ``` config.toml ``` are the current available parameters to control the *JoSIM* simulation.

## Notes

For IARPA contract SuperTools