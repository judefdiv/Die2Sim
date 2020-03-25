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
#include <time.h>
#include <cmath>
#include "die2sim/genFunc.hpp"

using namespace std;

class JoSimFile;
class PTLclass;
class CompClass;

constexpr auto c_ms = 299792458;

class JoSimFile{
	private:
		vector<string> fileNamesImport;
		vector<string> placeComp;
		vector<string> placeNet;

		string subcktName;
		vector<string> subcktNetName;	// Net name of the pad
		vector<string> subcktNetDes;	// Name of the pad

		vector<PTLclass> PTLs;
		vector<CompClass> comps;

		string createSubcktHeader();

	public:
		JoSimFile(){};

		int importCir(string fileName); // used to copy in subcircuits

		int createSubckt(string inName){subcktName = inName; return 1;};

		int pushComp(string name, string compTypeName, vector<string> &netNames);
		int pushPTL(const string &name, const string &netName, unsigned int len);

		int genCir(string fileName);
		void to_str();
};

class CompClass{
	private:
		string name;
		string compType;

		vector<string> nets;

	public:
		CompClass(){};
		~CompClass(){};

		int create(const string &CompName, const string &compTypeName, vector<string> netsIn);
		string to_cir();
};

class PTLclass{
	private:
		string name;
		string nameNet;

		int length;
		const double speedConstant = 3 * pow(10, 3) / c_ms;

	public:
		PTLclass(){};
		~PTLclass(){};

		int create(const string &PTLname, const string &NetName, int PTLlength);
		string to_cir();
};

int cpFile(string fromFile, string toFile);
string makeHeader(string HeaderName);
string makeFileHeader(string someText);

#endif