/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-04-25
 * Modified:
 * license:
 * Description: Passer to create a JoSIM file.
 * File:				ParserJosim.hpp
 */

#ifndef ParserJosim
#define ParserJosim

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <iomanip> // for string float precision
#include <sstream> // for string float precision
#include "die2sim/genFunc.hpp"

using namespace std;

class JoSimFile{
	private:
		vector<string> fileNamesImport;
		vector<string> placeComp;
		vector<string> placeNet;
		static int no_JoSim_classes;

		string subcktName;
		vector<string> subcktNetName;
		vector<string> subcktNetDes;

		string createSubcktHeader();

	public:
		JoSimFile(){no_JoSim_classes++;};

		// void createAuto(vector<vector<string> > &inBlock);
		int importCir(string fileName);
		int addComp(string compName, string compType, vector<string> &nets);
		int addTrans(string netName, vector<string> &nets, float impe, float tDelay);
		int createSubckt(string inName){subcktName = inName; return 1;};
		int appendSubcktPort(string netName, string netDes);

		int genCir(string fileName);
		void to_str();
};

int cpFile(string fromFile, string toFile);
// string makeHeader(string HeaderName);

#endif