/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-05-20
 * Modified:
 * license: 
 * Description: Reads in LEF/DEF/GDS files are displays the content.
 * File:				ToText.hpp
 */

#ifndef ToText
#define ToText

// #define configFile "config.toml"

// #include "toml/toml.hpp"
#include <string>
#include <iostream>

#include "die2sim/ParserJosim.hpp"
#include "die2sim/ParserDef.hpp"
#include "die2sim/ParserLef.hpp"
#include "die2sim/ParserGdsii.hpp"

#include "die2sim/genFunc.hpp"

using namespace std;

int decipherFile(string fileName);

#endif