/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-03-20
 * Modified:
 * license: 
 * Description: Parser for .def
 * File:				ParserDef.hpp
 */

#ifndef ParserDef
#define ParserDef

#include <iostream>				//stream
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include "die2sim/ClassDef.hpp"
#include "die2sim/genFunc.hpp"

using namespace std;

int defimport(string defFileName);
void createBlockDef(vector<vector<string> > &inBlock);
int proNewLineDef(vector<string> &inVec);

void disDEFdata();

void getNetVec(vector<def_net> &outVec);
void getSNetsVec(vector<def_specialNet> &outVec);

void getCompVec(vector<def_component> &outVec);

#endif