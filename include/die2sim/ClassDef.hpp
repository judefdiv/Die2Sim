/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-03-30
 * Modified:
 * license:
 * Description: Class for storing .def
 * File:				ClassDef.hpp
 */

#ifndef ClassDef
#define ClassDef

#include <string>
#include <iostream>
#include <vector>
#include <cmath> 		 // abs
#include <iomanip>
#include <set>
#include "die2sim/genFunc.hpp"

using namespace std;

class def_component;
class def_net;

vector<string> splitFileLine(ifstream &inFile);

class def_file{
	private:
		string name = "\0";

		string DESIGN;

		string unitsUnits = "\0";
		int unitsVar = 0;

		int DIEAREA[4] = {0};

		static int no_rows;
		static int no_tracks;

		vector<def_component> comps;
		vector<def_net> nets;
		vector<def_net> snets;

		set<string> validOHWords = {"DESIGN", "UNITS", "DIEAREA", "ROW", "TRACKS", "GCELLGRID"};
		set<string> validNBlkWords = {"COMPONENTS", "SPECIALNETS", "NETS"};

		void createAuto(vector<string> &inLine);

	public:
		def_file(){};
		~def_file(){};
		int importFile(string fileName);

		vector<def_component> getComps(){return this->comps;};
		vector<def_net> getNets(){return this->nets;};

		void to_str();
};

/**
 *  Components Class
 */

class def_component{
	private:
		string name = "\0";
		string compName = "\0";
		string posType = "\0";
		int pt[2] = {0};
		string orient = "\0";

	public:
		def_component(){};
		~def_component(){};

		int createAuto(vector<string> &inLine);

		string get_varName(){return name;}
		string get_varCompName(){return compName;}
		int get_varCorX(){return pt[0];}
		int get_varCorY(){return pt[1];}
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

	public:
		def_net(){};
		~def_net(){};

		int createAuto(vector<vector<string> > &inBlock);
		int createAutoSpecial(vector<vector<string> > &inBlock); // use with special nets

		void to_str();
		void get_route(vector<net_route> &VecOut){VecOut = this->routes;};
		string get_varName(){return name;}

		string get_varFromComp(){return fromComp;}
		string get_varFromPin(){return fromPin;}

		string get_varToComp(){return ToComp;}
		string get_varToPin(){return ToPin;}

		float get_trans_delay();
};

#endif