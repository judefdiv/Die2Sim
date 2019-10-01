/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-03-30
 * Modified:
 * license: 
 * Description: Class for storing .lef
 * File:				ClassLef.hpp
 */

#ifndef ClassLef
#define ClassLef

#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include "die2sim/genFunc.hpp"
#include <map>

using namespace std;

/** 
 *  Unit Class
 */

class lef_units{
	private:
		string database;
		int dbCount;
		int classID;

		static int no_unit_classes;

	public:
		lef_units();
		void createAuto(vector<vector<string> > &inBlock);
		static int get_varNoUnits() {return no_unit_classes;}
		string get_varDB() {return database;}
		int get_varDBcount() {return dbCount;}
		void to_str();
};


/**
 * 	Layer Class
 */

class lef_layer {
	private:
		int classID;
    string name;
    string TYPE;            // "ROUTING", "MASTERSLICE", "OVERLAP";
    string DIRECTION;       // {HORIZONTAL | VERTICAL} ;
    float PITCHx;          	// {distance | xDistance yDistance} ;
    float PITCHy;          	// {distance | xDistance yDistance} ;
    float WIDTH;            // defaultWidth ;
    float SPACING;
    //float OFFSET[];         // {distance | xDistance yDistance} ;
    
    static int no_layer_classes;
    static int no_layer_routing;
    static int no_layer_masterS;
    static int no_layer_overlap;
  public:
    lef_layer(vector<vector<string> > &inBlock);  // Class constructor 
    
    int get_varID(){ return classID;}
    string get_varName() { return name;}
    string get_varType() { return TYPE;}
    string get_varDirection() { return DIRECTION;}
    float get_varPitchX() { return PITCHx;}
    float get_varPitchy() { return PITCHy;}
    float get_varWidth() { return WIDTH;}

    void to_str();
};

/** 
 *  Via Class
 */

class lef_via{
	private:
		string name;
		int classID;
		bool DEFAULT;

		// 0 - top layer; 1 - middle layer; 2 - bottom layer
		// string LayerNames[3];
		// float layerDim[3][4];

		static int no_via_classes;

	public:
		// string name;
		string LayerNames[3];
		float layerDim[3][4];
		lef_via(vector<vector<string> > &inBlock);

		static int get_varNoUVia(){return no_via_classes;}
		string get_varName(){return name;}
		// void get_varLayerNames(string arrayIn){arrayIn = *LayerNames;}
		// void get_varLayerDim(float *arrayIn){arrayIn = *layerDim;}

		void to_str();
};

/** 
 *  Spacing Class
 */

class lef_spacing{
	private:
		string name;
		int classID;
		static int no_classes;

		int no_SAMENET = 0;
		vector<string> SAMENETlayer1;
		vector<string> SAMENETlayer2;
		vector<float> SAMENETdim;

	public:
		lef_spacing();
		void createAuto(vector<vector<string> > &inBlock);

		static int get_varNoCl() {return no_classes;}
		void to_str();
};

/** 
 *  Site Class
 */

class lef_site{
	private:
		string name;
		int classID;
		static int no_classes;

		float SIZE[2];
		string CLASS;
		string SYMMETRY;

	public:
		lef_site(vector<vector<string> > &inBlock);

		static int get_varNoCl() {return no_classes;}
		void to_str();
};

/** 
 *  Macro Class
 */

struct macro_port{
	string name = "\0";
	// int IDno;
	string type = "\0";
	float dim[4];
};

struct macro_pin{
	string name = "\0";
	// int IDno;
	string DIRECTION = "\0";
	vector<macro_port> PORT;

	void clear(){
		name = "\0";
		DIRECTION = "\0";
		PORT.clear();
	}
};

class lef_macro{
	private:
		string name;
		int classID;
		static int no_classes;

		float SIZE[2];
		float ORIGIN[2];
		string CLASS;
		string SYMMETRY;

		string SITE;
		float SITEpar[6];

		vector<macro_pin> PIN;
		vector<macro_port> OBS;

	public:
		lef_macro(vector<vector<string> > &inBlock);

		static int get_varNoCl(){return no_classes;}
		string get_varName(){return name;}
		float get_varSIZEX(){return SIZE[0];}
		float get_varSIZEY(){return SIZE[1];}
		float get_varORIGINX(){return ORIGIN[0];}
		float get_varORIGINY(){return ORIGIN[1];}
		void get_varPIN(vector<macro_pin> &VecOut){VecOut = PIN;};
		int get_noPIN(){return PIN.size();}
		void to_str();
};

#endif