# Die2Sim
Die2Sim is a tool used to aid simulations of large scale superconducting circuits. The main usage of the tool is to create GDSII and JoSIM files from LEF/DEF files using an existing cell library.

Due to the leniency of the syntax of the LEF/DEF file, Die2Sim was designed to use the files generated from Bookshelf (qPalace USC).


Version: 0.9

## Features
* LEF/DEF interpretation
* GDS to ASCII
* Easily create GDS file
* Importing GDS files
* Draw GDS file
* Create JoSIM files
* Convert LEF/DEF to GDS
* Convert LEF/DEF to JoSIM

## Examples
Examples of how to execute Die2Sim:

### LEF/DEF to JoSIM
``` bash
./Die2Sim -j lefExample.lef defExample.def -o josimOutput.cir
./Die2Sim -j lefExample.lef defExample.def # automatically assigns output filename
```
### Run Using Parameters from the config file
``` bash
./Die2Sim -c
```

## Needed stuffs
``` bash
apt install build-essencials 	# for compiling
```
## To Compile
``` bash
# Current directory: Die2Sim root
mkdir build && cd build
cmake ..
make
```


## Notes
Author - JF de Villiers (Stellenbosch University)
For IARPA contract SuperTools

LEF: Library Exchange Format

DEF: Design Exchange Format

JoSIM: Superconductor Circuit Simulator