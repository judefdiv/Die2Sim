/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-03-20
 * Modified:
 * license:
 * Description: Useful general functions.
 * File:				genFunc.hpp
 */

#ifndef genFunc
#define genFunc

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace std;

bool isStrDig(string wannaDig);
void printLineArrayF(float disArray[], int arrLen);
// bool checkFileExt(string fileName);


#define LEFmaxParam 14

vector<string> splitFileLine(ifstream &inFile);
void stringSplitVec(vector<string> &inVec, string inStr);
vector<string> SplitStrVec(string inStr);
void disVector(vector<string> &inVec);
void disVectorBlk(vector<vector<string> > &inBlock);
void progressBar(unsigned int currentVal, unsigned int totalVal);
string cleanWhite(string inStr);
string removeLTwhite(string inStr);
string fileRenamer(string inName, string preFix, string suffix);
string fileExtensionRenamer(string inName, string suffix);
string makeHeader(string HeaderName);

#endif