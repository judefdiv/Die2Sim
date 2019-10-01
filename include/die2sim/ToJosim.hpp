/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019--04-25
 * Modified:
 * license: 
 * Description: Runs the LEF/DEF files through JoSIM.
 * File:				ToJosim.hpp
 */

#ifndef joSim
#define joSim

#define configFile "config.toml"

#include "toml/toml.hpp"
#include <iostream>
#include <fstream>
#include <math.h>
// #include <unordered_map> 
#include <map>
#include "die2sim/ParserJosim.hpp"

#include "die2sim/ClassDef.hpp"
#include "die2sim/ParserDef.hpp"
// #include "lefClass.h"
// #include "lefPasser.h"

#include "die2sim/genFunc.hpp"
using namespace std;

int runJoSIM(string lefFileName, string defFileName, string cirFileName);

int stitchNets(string defFileName);

// int subcktPinLoc(string compName, string pinName);
int subcktPinNo(string cellName);
int subcktPinLoc(string cellName, string pinName);
string USC2LSmitll(string LSmitllStr);
string USC2LSmitllPin(string compName, string pinName);

int ptlStats(string defFileName);

#endif