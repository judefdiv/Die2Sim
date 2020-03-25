/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-03-30
 * Modified:
 * license:
 * Description: Class for storing .def
 * File:				ParserDef.hpp
 */

#ifndef ParserDef
#define ParserDef

class def_component;
class def_net;

#include <string>
#include <iostream>
#include <vector>
#include <cmath> 		 // abs
#include <iomanip>
#include <set>
#include <ctime>
#include "die2sim/genFunc.hpp"

using namespace std;

#define c_ms 299792458

vector<string> splitFileLine(ifstream &inFile);

class def_file{
	private:
		string name = "\0";

		string DESIGN;

		string unitsUnits = "\0";
		int unitsVar = 0;

		int DIEAREA[4] = {0};

		set<string> validOHWords = {"DESIGN", "UNITS", "DIEAREA", "ROW", "TRACKS", "GCELLGRID"};
		set<string> validNBlkWords = {"COMPONENTS", "SPECIALNETS", "NETS"};

		void createAuto(vector<string> &inLine);

	public:
		def_file(){};
		~def_file(){};

		vector<def_component> comps;
		vector<def_net> nets;
		vector<def_net> snets;

		int importFile(const string &fileName);
		// int importNodesNets(vector<BlifNode> inNodes,
  //                      vector<BlifNet> inNets);
		vector<def_component> getComps(){return this->comps;};
		vector<def_net> getNets(){return this->nets;};


		int to_def(const string &fileName);
		int to_jpg(const string &fileName);
		void to_str();
};

/**
 *  Components Class
 */

class def_component{
	private:

	public:
		def_component(){};
		~def_component(){};

		string name = "\0";
		string compName = "\0";
		string posType = "\0";
		// int pt[2] = {0};
		int corX = 0;
		int corY = 0;
		string orient = "\0";

		string compType = "\0"; // uhm?


		int createAuto(vector<string> &inLine);

		string getName(){return name;}
		string getCompType(){return compName;}
		int getCorX(){return corX;}
		int getCorY(){return corY;}
		string to_def(){
			string newType;
			if(!compName.compare("input") || !compName.compare("output")){
				newType = "PAD";
			}
			else{
				newType = compName;
			}
			return " - " + name + " "
									+ newType + " + FIXED ( "
									+ to_string(corX) + " "
									+ to_string(corY) + " ) N ;";
		}
		void to_str();
};

/**
 *  Nets Class
 */

struct net_route{
	string LAYER = "\0";
	string VIA = "\0";

	unsigned int trackWidth = 0;

	vector<int> ptX;
	vector<int> ptY;
};

class def_net{
	private:

		double speedConstant = 3 * pow(10, 3) / c_ms;

	public:
		def_net(){};
		~def_net(){};

		string name = "\0";

		// Master? compName pinName
		// Origin
		string fromComp = "\0";
		string fromPin = "\0";

		// MUSTJOIN (compName pinName)
		// Destination
		string ToComp = "\0";
		string ToPin = "\0";

		// tracks
		std::vector<net_route> routes;

		int createAuto(vector<vector<string> > &inBlock);
		int createAutoSpecial(vector<vector<string> > &inBlock); // use with special nets

		string to_def(){
			return "- " + name
					 + "\n( " + fromComp + " " + fromPin + " )"
					 + "\n( " + ToComp + " " + ToPin + " )"  + " ;";
		}
		void to_str();
		void get_route(vector<net_route> &VecOut){VecOut = this->routes;};
		string get_varName(){return name;}

		string get_varFromComp(){return fromComp;}
		string get_varFromPin(){return fromPin;}

		string get_varToComp(){return ToComp;}
		string get_varToPin(){return ToPin;}

		double get_trans_delay();
		unsigned int get_track_length();
};

#endif