# Die2Sim
Die2Sim is a tool used to aid simulations of large scale superconducting circuits. The main usage of Die2Sim is to create JoSIM files from a DEF file using an existing cell library.

Due to the leniency of the syntax of the LEF/DEF file, Die2Sim was designed to use the files generated from Bookshelf (qPalace USC).


Version: 0.9

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

#### Use CONFIG file

``` bash
./Die2Sim -c config.toml
```


## Notes

For IARPA contract SuperTools

LEF: Library Exchange Format

JoSIM: Superconductor Circuit Simulator