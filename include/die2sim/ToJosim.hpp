/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019--04-25
 * Modified:
 * license:
 * Description: Runs the LEF/DEF files through JoSIM.
 * File:				ToJosim.hpp
 */

#ifndef joSim
#define joSim

// #define configFile "config.toml"

#include "toml/toml.hpp"
#include <iostream>
#include <fstream>
#include <math.h>
#include <unordered_map>
#include <vector>
// #include <map>
//
#include "die2sim/ParserJosim.hpp"
#include "die2sim/ParserDef.hpp"
#include "die2sim/genFunc.hpp"

using namespace std;


int executeDef2Josim(string ConfigFileName, string DefFileName, string cirFileName);

class def2josim{
  private:
    JoSimFile joFile;

    // config file parameters and USC translation tables
    unordered_map<string, string> USC2LSmitllMap;
    unordered_map<string, string> NetListLoc;
    unordered_map<string, vector<string>> NetListPins;
    unordered_map<string, int> NetListPinNo;

    // config parameters
    string para_dangling_net  = "????";
    bool para_PTL_length_bool = false;
    float para_PTL_length     = -1;
    bool para_mergeIntoSubcir = true;
    vector<string> input_keys;
    vector<string> output_keys;
    vector<string> clock_keys;
    float time_step           = 0.1;
    float time_duration       = 1000;

    // Input pattern parameters
    float clock_freq    = 1.0;
    int input_peak      = 600;
    int input_peak_time = 35;

    // DEF file data
    vector<def_component> defComps;
    vector<def_net> defNets;

    // functions:
    int stitchCompNets();

    int subcktNoPins(string cellName);
    int subcktPinLoc(string cellName, string pinName);
    string USC2LSmitll(string USCstr);
    string USC2LSmitllPin(string compName, string pinName);

  public:
    def2josim(){};
    ~def2josim(){};

    int fetchData(string ConfigFileName, string DefFileName);
    int genCir(string fileName);
    int ptlStats();               // is this the right place for it?
};

#endif