/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-04-25
 * Modified:
 * license: 
 * Description: Runs the LEF/DEF files through JoSIM.
 * File:				ToJosim.cpp
 */

#include "die2sim/ToJosim.hpp"

JoSimFile joFile;

/**
 * [runJoSIM - Converts LEF/DEF to JoSIM .cir fiel for simulation]
 * @param  lefFileName [File name of the LEF file to be read]
 * @param  defFileName [File name of the DEF file to be read]
 * @param  cirFileName [File name of the cir file to be created]
 * @return             [1 - All good; 0 - error]
 */

int runJoSIM(string lefFileName, string defFileName, string cirFileName){
	cout << "Compiling LEF/DEF for JoSIM" << endl;

	const auto mainConfig  = toml::parse(configFile);
  auto loc = toml::get<map<string, string>>(mainConfig.at("LSmitll_GDS_File_Location"));

	joFile.setFileNameOut(cirFileName);
	joFile.importCir(loc["NETLIST"]);

	joFile.createSubckt("Created_subckt");
	stitchNets(defFileName);
	ptlStats(defFileName);
	// joFile.endSubckt();

	joFile.genCir();
	return 1;
}


/**
 * [stitchNets - Creates all the nets between the components(subckt)]
 * @param  defFileName [File name of the DEF file to be read]
 * @return             [1 - All good; 0 - error]
 */

int stitchNets(string defFileName){
	struct compNet{
		string compType = "";
		std::vector<string> nets;
	};

	vector<def_component> cl_comps;	
	vector<def_net> def_cl_nets;

	defimport(defFileName);
	getCompVec(cl_comps);	
	getNetVec(def_cl_nets);

	cout << "Mapping nets." << endl;
	
	vector<string> net;

	// unordered_map<string, compNet> nets;
	map<string, compNet> nets;
	compNet tempCompNet;

	cout << "Drawing component map" << endl;
	for(unsigned int i = 0; i < cl_comps.size(); i++){
		// progressBar( i, cl_comps.size());
		int numberpins = subcktPinNo(USC2LSmitll(cl_comps[i].get_varCompName()));
	
		net.clear();
		net.resize(numberpins);

		for(int j = 0; j < numberpins; j++){
			net[j] = "????";
		}

		tempCompNet.compType = cl_comps[i].get_varCompName();
		tempCompNet.nets = net;

		nets[cl_comps[i].get_varName()] = tempCompNet;
	}

	net.clear();
	net.resize(4);

	cout << "Stitching nets to component map" << endl;
	for(unsigned int i = 0; i < def_cl_nets.size(); i++){
		int index;
		// cout << "net: " << def_cl_nets[i].get_varName() << endl;
		// from
		auto nets_it = nets.find(def_cl_nets[i].get_varFromComp());
		if(nets_it != nets.end()){
			// index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitllPin(nets_it->second.compType, def_cl_nets[i].get_varFromPin()));
			index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitll(def_cl_nets[i].get_varFromPin()));
	    // nets_it->second.nets[index] = def_cl_nets[i].get_varName() + "A";
	    nets_it->second.nets[index] = to_string(i) + "A";

	  } else{
	  	cout << "Component \"" << def_cl_nets[i].get_varFromComp() << "\" not found." << endl;
	  }

		// must join
		nets_it = nets.find(def_cl_nets[i].get_varToComp());
		if(nets_it != nets.end()){
			// index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitllPin(nets_it->second.compType, def_cl_nets[i].get_varToPin()));
			index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitll(def_cl_nets[i].get_varToPin()));
	    // nets_it->second.nets[index] = def_cl_nets[i].get_varName() + "B";
	    nets_it->second.nets[index] = to_string(i) + "B";

	  } else{
	  	cout << "Component \"" << def_cl_nets[i].get_varToComp() << "\" not found." << endl;
	  }
	  
	  net[0] = to_string(i) + "A";
	  net[1] = "0";
	  net[2] = to_string(i) + "B";
	  net[3] = "0";
	  // addTrans(string netName, vector<string> &nets, float impe, float tDelay)
	  joFile.addTrans(def_cl_nets[i].get_varName(), net, 5, def_cl_nets[i].get_trans_delay());
	}

	cout << "Adding component to JoSIM class" << endl;
	for(auto& it: nets){
		joFile.addComp(it.first, USC2LSmitll(it.second.compType), it.second.nets);
	}

	return 1;
}

/**
 * [subcktPinLoc - Helps generate the netlist component with the correct pin order]
 * @param  cellName [The name of the component]
 * @param  pinName  [The name of the pin]
 * @return          [The order of the pin location]
 */

int subcktPinLoc(string cellName, string pinName){
	static bool firstRUN = false;
	static map<string, vector<string>> LSmitll_subckt;
	vector<string> foo;

	if(!firstRUN){
		firstRUN = true;
	  const auto mainConfig  = toml::parse(configFile);
	  LSmitll_subckt = toml::get<map<string, vector<string>>>(mainConfig.at("LSmitll_netlist"));
	}

	if(LSmitll_subckt.find(cellName) == LSmitll_subckt.end()){			// remove for speed increase
		cout << "Cell not found: \"" << cellName << "\"" << endl;
		return 0;
	}

	foo = LSmitll_subckt.find(cellName)->second;


	if(cellName == "MITLL_SPLITT" && pinName == "IN_CLK"){
		pinName = "IN";
	}


	for(unsigned int i = 0; i < foo.size(); i++){
		if(pinName == foo[i]){
			return i;
		}
	}
	cout << "pin: \"" << pinName << "\" NOT FOUND in \"" <<  cellName << "\""<< endl;
	return 0;
}

/**
 * [subcktPinNo - Finds the total number of pins]
 * @param  cellName [The name of the component]
 * @return          [The total number of pins]
 */

int subcktPinNo(string cellName){
	static bool firstRUN = false;
	static map<string, int> NetListPinNo;

	if(!firstRUN){
		firstRUN = true;
	  const auto mainConfig  = toml::parse(configFile);
	  map<string, vector<string>> subckt = toml::get<map<string, vector<string>>>(mainConfig.at("LSmitll_netlist"));

  	map<string, vector<string>>::iterator it;

		for(it = subckt.begin(); it != subckt.end(); it++){
		  NetListPinNo.insert(pair<string,int>(it->first, it->second.size()));
		}
	}


	if(NetListPinNo.find(cellName) == NetListPinNo.end()){			// remove for speed increase
		cout << "Cell not found: \"" << cellName << "\"" << endl;
		return 0;
	}

	return NetListPinNo.find(cellName)->second;
}

/**
 * [USC2LSmitll - Converts USC naming scheme to LSmitll naming scheme]
 * @param  USCstr  [USC name]
 * @return         [LSmitll name]
 */

 string USC2LSmitll(string USCstr){
	static bool firstRUN = false;
	static map<string, string> USC2LSmitll;

	if(!firstRUN){
		firstRUN = true;
	  const auto mainConfig  = toml::parse(configFile);
	  USC2LSmitll = toml::get<map<string, string>>(mainConfig.at("USC2LSmitll"));
	}

	if(USC2LSmitll.find(USCstr) == USC2LSmitll.end()){			// remove for speed increase
		cout << "Key not found: \"" << USCstr << "\"" << endl;
		return "\0";
	}

	return USC2LSmitll.find(USCstr)->second;
}

/**
 * [USC2LSmitllPin - Converts USC naming scheme to LSmitll naming scheme]
 * @param  compName [USC name of the cell]
 * @param  pinName  [USC name of the pin]
 * @return          [LSmitll name]
 */

string USC2LSmitllPin(string compName, string pinName){
	static bool firstRUN = false;
	static map<string, string> USC2LSmitll;

	if(compName == "MITLL_SPLITT" && pinName == "IN_CLK"){
		return "IN";
	}

	if(!firstRUN){
		firstRUN = true;
	  const auto mainConfig  = toml::parse(configFile);
	  USC2LSmitll = toml::get<map<string, string>>(mainConfig.at("USC2LSmitll"));
	}

	if(USC2LSmitll.find(pinName) == USC2LSmitll.end()){			// remove for speed increase
		cout << "Key not found: \"" << pinName << "\"" << endl;
		return "\0";
	}

	return USC2LSmitll.find(pinName)->second;


	// --------------------------------------------------------------------------------------------

	// map<string, string> USC2LSmitll;
	
	// cout << "trans" << endl;

	// static const auto mainConfig  = toml::parse(configFile);
	// static const auto USC2LSmitllTable = toml::get<std::vector<toml::table>>(mainConfig.at("USC2LSmitllTable"));

	// unsigned int i;
	// for(i = 0; i < USC2LSmitllTable.size(); i++){
	// 	cout << "[" << i << "]:" << endl;
	// 	USC2LSmitll = toml::get<std::map<std::string, std::string>>(USC2LSmitllTable[i]);
	// 	if(USC2LSmitll["name"] == compName){
	// 		auto USC2LSmitll_it = USC2LSmitll.find(pinName);
	// 		if(USC2LSmitll_it == USC2LSmitll.end()){			// remove for speed increase
	// 			cout << "pin: \"" << pinName << "\" NOT FOUND in \"" <<  compName << "\""<< endl;
	// 			return "\0";
	// 		}
	// 		return USC2LSmitll_it->second;
	// 	}
	// }

	// cout << "Translating: \"" << compName << "\" FAILED" << endl;
	// return "\0";

	// ---------------------------------------------------------------------------------------------

	// static bool firstRUN = false;
	// static map<string, vector<string>> USC2LSmitll;

	// if(!firstRUN){
	// 	firstRUN = true;
	//   const auto mainConfig  = toml::parse(configFile);
	//   USC2LSmitll = toml::get<map<string, vector<string>>>(mainConfig.at("USC2LSmitllTable"));
	// }

	// auto USC2LSmitll_it = USC2LSmitll.find(compName);

	// if(USC2LSmitll_it == USC2LSmitll.end()){			// remove for speed increase
	// 	cout << "Key not found: \"" << USC2LSmitll_it->first << "\"" << endl;
	// 	return "\0";
	// }

	// for(unsigned int i = 0; i < USC2LSmitll_it->second.size(); i++){
	// 	if(USC2LSmitll_it->second[i] == pinName){
	// 		return USC2LSmitll_it->second[i];
	// 	}
	// }

	// cout << "pin: \"" << pinName << "\" NOT FOUND in \"" <<  compName << "\""<< endl;
	// return "\0";
}

/**
 * [ptlStats - Calculates the stats of the route/PTLs in DEF file]
 * @param    defFileName [File name of the DEF file to be read]
 * @return	   					 [1 - All good; 0 - error]
 */

int ptlStats(string defFileName){
	vector<def_net> def_cl_nets;
	// defimport(defFileName); // already called
	getNetVec(def_cl_nets);

	unsigned int lenMax = 0;				// Maximum length of the PTLs
	unsigned int lenMin = 10000;		// Minimum length of the PTLs
	float lenMean = 0;							// The mean/average length of the PTLs
	unsigned int lenCnt = 0;				// Number of PTLs
	unsigned long lenSum = 0;				// The sum of the PTL length
	float stdVar = 0; 							// Standards variance


	cout << "Calculating stats on the transmission lines." << endl;
	lenCnt = def_cl_nets.size();
	for(unsigned int i = 0; i < lenCnt; i++){
	  lenSum += def_cl_nets[i].get_trans_delay();

	  if(def_cl_nets[i].get_trans_delay() > lenMax)
	  	lenMax = def_cl_nets[i].get_trans_delay();

	  if(def_cl_nets[i].get_trans_delay() < lenMin)
	  	lenMin = def_cl_nets[i].get_trans_delay();
	}

	lenMean = (float)lenSum / (float)lenCnt;

	for(unsigned int i = 0; i < lenCnt; i++){
	  stdVar += pow((float)def_cl_nets[i].get_trans_delay() - lenMean, 2);
	}

	stdVar /= lenCnt;

	stdVar = sqrt(stdVar);

	cout << "PTLs:" << endl;
	cout << "\tCnt: "  << lenCnt << endl;
	cout << "\tMin: "  << lenMin << endl;
	cout << "\tMean: "  << lenMean << endl;
	cout << "\tMax: "  << lenMax << endl;
	cout << "\tTotal: "  << lenSum << endl;
	cout << "\tstdVar: "  << stdVar << endl;

	return 1;
}