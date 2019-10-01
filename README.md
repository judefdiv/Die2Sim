# Die2Sim
Die2Sim is a tool used to aid simulations of large scale superconducting circuits. The main usage of the tool is to create GDSII and JoSIM files from LEF/DEF files using an existing cell library.

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

### Interpret Files
``` bash
./Die2Sim -i gdsExample.gdsii
./Die2Sim -i lefExample.lef
./Die2Sim -i defExample.def
```
### LEF/DEF to GDSII
``` bash
./Die2Sim -g lefExample.lef defExample.def -o gdsOutput.gds
./Die2Sim -g lefExample.lef defExample.def # automatically assigns output filename
```
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
apt install libreadline-dev 	# for Berkeley ABC
# apt install gv graphviz				# to visualise ABC's logic flow
apt install dot 							# To replace ABC's logic flow visuals
```
## To Compile
``` bash
# Current directory: Die2Sim root
mkdir build && cd build
cmake ..
make
```

## ABC Modifications
``` cpp
// in "abc/src/opt/dau/dau.h" added:
extern void		Abc_TtVerifySmallTruth(word * pTruth, int nVars);
extern int 		shiftFunc(int ci);
```

## Notes
Author - JF de Villiers (Stellenbosch University)
For IARPA contract SuperTools

LEF: Library Exchange Format

DEF: Design Exchange Format

GDSII: Graphic Database System

JoSIM: Superconductor Circuit Simulator

BLIF: The Berkeley Logic Interchange Format