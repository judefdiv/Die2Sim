/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-04-25
 * Modified:
 * license: 
 * Description: Parser for .LEF
 * File:				ParserLef.cpp
 */

#ifndef ParserLef
#define ParserLef

#include <iostream>				//stream
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include "die2sim/ClassLef.hpp"
#include "die2sim/genFunc.hpp"

using namespace std;

int lefimport(string LefFileName);
int createBlockLef(vector<string> &inVec, vector<vector<string> > &strBlock);
int proNewLineLef(vector<string> &inVec);

int getLayerNo(string LayerName);
int getTrackWidth(string LayerName);

void disLEFdata();

// void getNetVec(vector<def_net> &outVec);
// void getSNetsVec(vector<def_specialNet> &outVec);
void getViaVec(vector<lef_via> &outVec);
void getMacroVec(vector<lef_macro> &outVec);

#endif