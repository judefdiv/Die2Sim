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
#include "die2sim/genFunc.hpp"

using namespace std;

/** 
 *  OverHeads Class
 */

struct OH_row{
	string name = "\0";
	string siteName = "\0";
	int origX = -1;
	int origY = -1;
	int numX = -1;
	int numY = -1;
	int stepX = -1;
	int stepY = -1;
};

struct OH_tracks{
	string axis = "\0";
	int start = -1;
	int numtracks = -1;
	int space = -1;
	string LAYER = "\0";
};

class def_OverHeads{
	private:
//		int classID;
		string name = "\0";

		string DESIGN;

		string unitsUnits = "\0";
		int unitsVar = 0;

		int DIEAREA[4] = {0};

		static int no_rows;
		static int no_tracks;

		// GCELLGRID
		int GCGx[3] = {0};
		int GCGy[3] = {0};

		vector<OH_row> ROW;
		vector<OH_tracks> TRACKS;

	public:
		def_OverHeads();
		void createAuto(vector<string> &inLine);
		void to_str();
};

/** 
 *  Components Class
 */

class def_component{
	private:
		static int compCnt;
		int classID;
		string name = "\0";
		string compName = "\0";
		string posType = "\0";
		int pt[2] = {0};
		string orient = "\0";

	public:
		def_component(vector<string> &inLine);
		// void createAuto(vector<string> &inLine);
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

	int trackWidth = -1;

	vector<int> ptX;
	vector<int> ptY;
};

class def_net{
	private:
		int classID;
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

		static int netCnt;
	public:
		def_net(vector<vector<string> > &inBlock);
		// void createAuto(vector<string> &inLine);
		void to_str();
		void get_route(vector<net_route> &VecOut){VecOut = this->routes;};
		string get_varName(){return name;}

		string get_varFromComp(){return fromComp;}
		string get_varFromPin(){return fromPin;}

		string get_varToComp(){return ToComp;}
		string get_varToPin(){return ToPin;}

		float get_trans_delay();
};

/** 
 *  Specialnets Class
 */

class def_specialNet{
	private:
		int classID;
		string name = "\0";

		// tracks
		std::vector<net_route> routes;

		static int netCnt;
	public:
		def_specialNet(vector<vector<string> > &inBlock);
		// void createAuto(vector<string> &inLine);
		void to_str();
		void get_route(vector<net_route> &VecOut){VecOut = this->routes;};
		string get_varName(){return name;}
};

#endif