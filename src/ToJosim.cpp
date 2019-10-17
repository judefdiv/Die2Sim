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

/**
 * [executeDef2Josim - A script for converting DEF file to JoSIM]
 * @param  ConfigFileName [The config.toml file]
 * @param  DefFileName    [The .def file]
 * @param  cirFileName    [The outfile JoSIm file (.cir)]
 * @return                [0 - Exit Success; 1 - Exit Failure]
 */

int executeDef2Josim(string ConfigFileName, string DefFileName, string cirFileName){
	cout << "Compiling LEF/DEF for JoSIM" << endl;

	def2josim defJosim;

	defJosim.fetchData(ConfigFileName, DefFileName);
	// defJosim.stitchCompNets();
	defJosim.genCir(cirFileName);
	// defJosim.ptlStats();

	return 1;
}


/**
 * [def2josim::fetchData - gets the data from the appropriate places]
 * @param  ConfigFileName [The config.toml file]
 * @param  DefFileName    [The .def file name]
 * @return                [0 - Exit Success; 1 - Exit Failure]
 */

int def2josim::fetchData(string ConfigFileName, string DefFileName){

	const auto mainConfig  = toml::parse(ConfigFileName);
  this->NetListLoc = toml::get<unordered_map<string, string>>(mainConfig.at("NETLIST_LOCATIONS"));
  this->USC2LSmitllMap = toml::get<unordered_map<string, string>>(mainConfig.at("USC2LSmitll"));
  this->NetListPins = toml::get<unordered_map<string, vector<string>>>(mainConfig.at("LSmitll_netlist"));

  // Creates a map with the number of pins of each sub-circuit
	unordered_map<string, vector<string>>::iterator it;
	for(it = this->NetListPins.begin(); it != this->NetListPins.end(); it++){
	  this->NetListPinNo.insert(pair<string,int>(it->first, it->second.size()));
	}

	def_file defFileIn;
	defFileIn.importFile(DefFileName);

	this->defComps = defFileIn.getComps();
	this->defNets = defFileIn.getNets();

	return 0;
}

/**
 * [def2josim::genCir - Compiling DEF for JoSIM]
 * @param  fileName [The output file name]
 * @return          [0 - Exit Success; 1 - Exit Failure]
 */

int def2josim::genCir(string fileName){
	cout << "Compiling DEF for JoSIM" << endl;

	for(const auto & [netName, netLoc]: this->NetListLoc){
	  joFile.importCir(netLoc);
	}

	joFile.createSubckt("Created_subckt");

	this->stitchCompNets();

	joFile.genCir(fileName);

	return 0;
}

/**
 * [def2josim::stitchCompNets Creates all the nets between the components(subckt)]
 * @return [0 - Exit Success; 1 - Exit Failure]
 */

int def2josim::stitchCompNets(){

	struct compNet{
		string compType = "";
		std::vector<string> nets;
	};


	cout << "Mapping nets." << endl;

	vector<string> net;

	unordered_map<string, compNet> nets; // Name of the component, the nets
	compNet tempCompNet;

	cout << "Drawing component map" << endl;
	// creating question marks on all the nets so the user can see unused nets
	int numberpins;
	for(auto &comp: this->defComps){
		numberpins = this->subcktNoPins(USC2LSmitll(comp.get_varCompName()));

		// net.clear();
		net.resize(numberpins);

		for(int j = 0; j < numberpins; j++){
			net[j] = "????";
		}

		tempCompNet.compType = comp.get_varCompName();
		tempCompNet.nets = net;
		nets[comp.get_varName()] = tempCompNet;
	}

	net.clear();
	net.resize(4);

	cout << "Stitching nets to component map" << endl;
	// for(unsigned int i = 0; i < def_cl_nets.size(); i++){
	int i = 0;
	for(auto &itNet: this->defNets){
		int index;
		// from
		auto nets_it = nets.find(itNet.get_varFromComp());
		if(nets_it != nets.end()){
			index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitll(itNet.get_varFromPin()));
	    nets_it->second.nets[index] = to_string(i) + "A";

	  } else{
	  	cout << "Component \"" << itNet.get_varFromComp() << "\" not found." << endl;
	  }

		// must join
		nets_it = nets.find(itNet.get_varToComp());
		if(nets_it != nets.end()){
			index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitll(itNet.get_varToPin()));
	    nets_it->second.nets[index] = to_string(i) + "B";

	  } else{
	  	cout << "Component \"" << itNet.get_varToComp() << "\" not found." << endl;
	  }

	  net[0] = to_string(i) + "A";
	  net[1] = "0";
	  net[2] = to_string(i) + "B";
	  net[3] = "0";

	  joFile.addTrans(itNet.get_varName(), net, 5, itNet.get_trans_delay());
	  i++;
	}

	cout << "Adding component to JoSIM class" << endl;

	for(auto &[compName, data]: nets){
		joFile.addComp(compName, USC2LSmitll(data.compType), data.nets);
	}

	return 0;
}

/**
 * [def2josim::ptlStats description]
 * @return [0 - Exit Success; 1 - Exit Failure]
 */

int def2josim::ptlStats(){
	// 	vector<def_net> def_cl_nets;
	// // defimport(defFileName); // already called
	// getNetVec(def_cl_nets);

	// unsigned int lenMax = 0;				// Maximum length of the PTLs
	// unsigned int lenMin = 10000;		// Minimum length of the PTLs
	// float lenMean = 0;							// The mean/average length of the PTLs
	// unsigned int lenCnt = 0;				// Number of PTLs
	// unsigned long lenSum = 0;				// The sum of the PTL length
	// float stdVar = 0; 							// Standards variance


	// cout << "Calculating stats on the transmission lines." << endl;
	// lenCnt = def_cl_nets.size();
	// for(unsigned int i = 0; i < lenCnt; i++){
	//   lenSum += def_cl_nets[i].get_trans_delay();

	//   if(def_cl_nets[i].get_trans_delay() > lenMax)
	//   	lenMax = def_cl_nets[i].get_trans_delay();

	//   if(def_cl_nets[i].get_trans_delay() < lenMin)
	//   	lenMin = def_cl_nets[i].get_trans_delay();
	// }

	// lenMean = (float)lenSum / (float)lenCnt;

	// for(unsigned int i = 0; i < lenCnt; i++){
	//   stdVar += pow((float)def_cl_nets[i].get_trans_delay() - lenMean, 2);
	// }

	// stdVar /= lenCnt;

	// stdVar = sqrt(stdVar);

	// cout << "PTLs:" << endl;
	// cout << "\tCnt: "  << lenCnt << endl;
	// cout << "\tMin: "  << lenMin << endl;
	// cout << "\tMean: "  << lenMean << endl;
	// cout << "\tMax: "  << lenMax << endl;
	// cout << "\tTotal: "  << lenSum << endl;
	// cout << "\tstdVar: "  << stdVar << endl;

	return 0;
}

/**
 * [def2josim::USC2LSmitll - A translating function]
 * @param  USCstr [The string to be translated]
 * @return        [The translated string]
 */

string def2josim::USC2LSmitll(string USCstr){
	unordered_map<string, string>::iterator foo;

	foo = this->USC2LSmitllMap.find(USCstr);

	if(foo == this->USC2LSmitllMap.end()){
		cout << "Key not found: \"" << USCstr << "\"" << endl;
		return "\0";
	}

	return foo->second;
}

/**
 * [def2josim::USC2LSmitllPin - Thank you USC for this special translating function]
 * @param  compName [The special string]
 * @param  pinName  [The string to be translated]
 * @return          [The translated string]
 */

string def2josim::USC2LSmitllPin(string compName, string pinName){
	if(compName == "MITLL_SPLITT" && pinName == "IN_CLK"){
		return "IN";
	}

	unordered_map<string, string>::iterator foo;

	foo = this->USC2LSmitllMap.find(pinName);

	if(foo == this->USC2LSmitllMap.end()){
		cout << "Key not found: \"" << pinName << "\"" << endl;
		return "\0";
	}

	return foo->second;
}

/**
 * [def2josim:: - Helps generate the netlist component with the correct pin order]
 * @param  cellName [The name of the component]
 * @param  pinName  [The name of the pin]
 * @return          [The order of the pin location]
 */

int def2josim::subcktPinLoc(string cellName, string pinName){
	vector<string> foo;

	unordered_map<string, vector<string>>::iterator it;

	it = this->NetListPins.find(cellName);

	if(it == this->NetListPins.end()){
		cout << "Cell not found: \"" << cellName << "\"" << endl;
		return 0;
	}

	foo = it->second;

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
 * [def2josim::subcktNoPins - Get the number of pins that a sub-circuit has]
 * @param  cellName [The name of the subcircuit]
 * @return          [The number of pins that the subcir has]
 */

int def2josim::subcktNoPins(string cellName){
	unordered_map<string, int>::iterator foo;

	foo = this->NetListPinNo.find(cellName);

	if(foo == this->NetListPinNo.end()){
		cout << "Cell not found: \"" << cellName << "\"" << endl;
		return 0;
	}

	return NetListPinNo.find(cellName)->second;
}

















// -----------------------------------------------------------------------
// ------------------------------- Legacy --------------------------------
// -----------------------------------------------------------------------


// JoSimFile joFile;

// /**
//  * [runJoSIM - Converts LEF/DEF to JoSIM .cir fiel for simulation]
//  * @param  lefFileName [File name of the LEF file to be read]
//  * @param  defFileName [File name of the DEF file to be read]
//  * @param  cirFileName [File name of the cir file to be created]
//  * @return             [1 - All good; 0 - error]
//  */

// int runJoSIM(string lefFileName, string defFileName, string cirFileName){
// 	cout << "Compiling LEF/DEF for JoSIM" << endl;

// 	const auto mainConfig  = toml::parse(configFile);
//   auto loc = toml::get<map<string, string>>(mainConfig.at("LSmitll_GDS_File_Location"));

// 	joFile.setFileNameOut(cirFileName);
// 	joFile.importCir(loc["NETLIST"]);

// 	joFile.createSubckt("Created_subckt");
// 	stitchNets(defFileName);
// 	ptlStats(defFileName);
// 	// joFile.endSubckt();

// 	joFile.genCir();
// 	return 1;
// }


// /**
//  * [stitchNets - Creates all the nets between the components(subckt)]
//  * @param  defFileName [File name of the DEF file to be read]
//  * @return             [1 - All good; 0 - error]
//  */

// int stitchNets(string defFileName){
// 	struct compNet{
// 		string compType = "";
// 		std::vector<string> nets;
// 	};

// 	vector<def_component> cl_comps;
// 	vector<def_net> def_cl_nets;

// 	defimport(defFileName);
// 	getCompVec(cl_comps);
// 	getNetVec(def_cl_nets);

// 	cout << "Mapping nets." << endl;

// 	vector<string> net;

// 	// unordered_map<string, compNet> nets;
// 	map<string, compNet> nets;
// 	compNet tempCompNet;

// 	cout << "Drawing component map" << endl;
// 	for(unsigned int i = 0; i < cl_comps.size(); i++){
// 		// progressBar( i, cl_comps.size());
// 		int numberpins = subcktPinNo(USC2LSmitll(cl_comps[i].get_varCompName()));

// 		net.clear();
// 		net.resize(numberpins);

// 		for(int j = 0; j < numberpins; j++){
// 			net[j] = "????";
// 		}

// 		tempCompNet.compType = cl_comps[i].get_varCompName();
// 		tempCompNet.nets = net;

// 		nets[cl_comps[i].get_varName()] = tempCompNet;
// 	}

// 	net.clear();
// 	net.resize(4);

// 	cout << "Stitching nets to component map" << endl;
// 	for(unsigned int i = 0; i < def_cl_nets.size(); i++){
// 		int index;
// 		// cout << "net: " << def_cl_nets[i].get_varName() << endl;
// 		// from
// 		auto nets_it = nets.find(def_cl_nets[i].get_varFromComp());
// 		if(nets_it != nets.end()){
// 			// index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitllPin(nets_it->second.compType, def_cl_nets[i].get_varFromPin()));
// 			index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitll(def_cl_nets[i].get_varFromPin()));
// 	    // nets_it->second.nets[index] = def_cl_nets[i].get_varName() + "A";
// 	    nets_it->second.nets[index] = to_string(i) + "A";

// 	  } else{
// 	  	cout << "Component \"" << def_cl_nets[i].get_varFromComp() << "\" not found." << endl;
// 	  }

// 		// must join
// 		nets_it = nets.find(def_cl_nets[i].get_varToComp());
// 		if(nets_it != nets.end()){
// 			// index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitllPin(nets_it->second.compType, def_cl_nets[i].get_varToPin()));
// 			index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitll(def_cl_nets[i].get_varToPin()));
// 	    // nets_it->second.nets[index] = def_cl_nets[i].get_varName() + "B";
// 	    nets_it->second.nets[index] = to_string(i) + "B";

// 	  } else{
// 	  	cout << "Component \"" << def_cl_nets[i].get_varToComp() << "\" not found." << endl;
// 	  }

// 	  net[0] = to_string(i) + "A";
// 	  net[1] = "0";
// 	  net[2] = to_string(i) + "B";
// 	  net[3] = "0";
// 	  // addTrans(string netName, vector<string> &nets, float impe, float tDelay)
// 	  joFile.addTrans(def_cl_nets[i].get_varName(), net, 5, def_cl_nets[i].get_trans_delay());
// 	}

// 	cout << "Adding component to JoSIM class" << endl;
// 	for(auto& it: nets){
// 		joFile.addComp(it.first, USC2LSmitll(it.second.compType), it.second.nets);
// 	}

// 	return 1;
// }

// /**
//  * [subcktPinLoc - Helps generate the netlist component with the correct pin order]
//  * @param  cellName [The name of the component]
//  * @param  pinName  [The name of the pin]
//  * @return          [The order of the pin location]
//  */

// int subcktPinLoc(string cellName, string pinName){
// 	static bool firstRUN = false;
// 	static map<string, vector<string>> LSmitll_subckt;
// 	vector<string> foo;

// 	if(!firstRUN){
// 		firstRUN = true;
// 	  const auto mainConfig  = toml::parse(configFile);
// 	  LSmitll_subckt = toml::get<map<string, vector<string>>>(mainConfig.at("LSmitll_netlist"));
// 	}

// 	if(LSmitll_subckt.find(cellName) == LSmitll_subckt.end()){			// remove for speed increase
// 		cout << "Cell not found: \"" << cellName << "\"" << endl;
// 		return 0;
// 	}

// 	foo = LSmitll_subckt.find(cellName)->second;


// 	if(cellName == "MITLL_SPLITT" && pinName == "IN_CLK"){
// 		pinName = "IN";
// 	}


// 	for(unsigned int i = 0; i < foo.size(); i++){
// 		if(pinName == foo[i]){
// 			return i;
// 		}
// 	}
// 	cout << "pin: \"" << pinName << "\" NOT FOUND in \"" <<  cellName << "\""<< endl;
// 	return 0;
// }

// /**
//  * [subcktPinNo - Finds the total number of pins]
//  * @param  cellName [The name of the component]
//  * @return          [The total number of pins]
//  */

// int subcktPinNo(string cellName){
// 	static bool firstRUN = false;
// 	static map<string, int> NetListPinNo;

// 	if(!firstRUN){
// 		firstRUN = true;
// 	  const auto mainConfig  = toml::parse(configFile);
// 	  map<string, vector<string>> subckt = toml::get<map<string, vector<string>>>(mainConfig.at("LSmitll_netlist"));

//   	map<string, vector<string>>::iterator it;

// 		for(it = subckt.begin(); it != subckt.end(); it++){
// 		  NetListPinNo.insert(pair<string,int>(it->first, it->second.size()));
// 		}
// 	}


// 	if(NetListPinNo.find(cellName) == NetListPinNo.end()){			// remove for speed increase
// 		cout << "Cell not found: \"" << cellName << "\"" << endl;
// 		return 0;
// 	}

// 	return NetListPinNo.find(cellName)->second;
// }

// /**
//  * [USC2LSmitll - Converts USC naming scheme to LSmitll naming scheme]
//  * @param  USCstr  [USC name]
//  * @return         [LSmitll name]
//  */

// string USC2LSmitll(string USCstr){
// 	static bool firstRUN = false;
// 	static map<string, string> USC2LSmitll;

// 	if(!firstRUN){
// 		firstRUN = true;
// 	  const auto mainConfig  = toml::parse(configFile);
// 	  USC2LSmitll = toml::get<map<string, string>>(mainConfig.at("USC2LSmitll"));
// 	}

// 	if(USC2LSmitll.find(USCstr) == USC2LSmitll.end()){			// remove for speed increase
// 		cout << "Key not found: \"" << USCstr << "\"" << endl;
// 		return "\0";
// 	}

// 	return USC2LSmitll.find(USCstr)->second;
// }

// /**
//  * [USC2LSmitllPin - Converts USC naming scheme to LSmitll naming scheme]
//  * @param  compName [USC name of the cell]
//  * @param  pinName  [USC name of the pin]
//  * @return          [LSmitll name]
//  */

// string USC2LSmitllPin(string compName, string pinName){
// 	static bool firstRUN = false;
// 	static map<string, string> USC2LSmitll;

// 	if(compName == "MITLL_SPLITT" && pinName == "IN_CLK"){
// 		return "IN";
// 	}

// 	if(!firstRUN){
// 		firstRUN = true;
// 	  const auto mainConfig  = toml::parse(configFile);
// 	  USC2LSmitll = toml::get<map<string, string>>(mainConfig.at("USC2LSmitll"));
// 	}

// 	if(USC2LSmitll.find(pinName) == USC2LSmitll.end()){			// remove for speed increase
// 		cout << "Key not found: \"" << pinName << "\"" << endl;
// 		return "\0";
// 	}

// 	return USC2LSmitll.find(pinName)->second;


// 	// --------------------------------------------------------------------------------------------

// 	// map<string, string> USC2LSmitll;

// 	// cout << "trans" << endl;

// 	// static const auto mainConfig  = toml::parse(configFile);
// 	// static const auto USC2LSmitllTable = toml::get<std::vector<toml::table>>(mainConfig.at("USC2LSmitllTable"));

// 	// unsigned int i;
// 	// for(i = 0; i < USC2LSmitllTable.size(); i++){
// 	// 	cout << "[" << i << "]:" << endl;
// 	// 	USC2LSmitll = toml::get<std::map<std::string, std::string>>(USC2LSmitllTable[i]);
// 	// 	if(USC2LSmitll["name"] == compName){
// 	// 		auto USC2LSmitll_it = USC2LSmitll.find(pinName);
// 	// 		if(USC2LSmitll_it == USC2LSmitll.end()){			// remove for speed increase
// 	// 			cout << "pin: \"" << pinName << "\" NOT FOUND in \"" <<  compName << "\""<< endl;
// 	// 			return "\0";
// 	// 		}
// 	// 		return USC2LSmitll_it->second;
// 	// 	}
// 	// }

// 	// cout << "Translating: \"" << compName << "\" FAILED" << endl;
// 	// return "\0";

// 	// ---------------------------------------------------------------------------------------------

// 	// static bool firstRUN = false;
// 	// static map<string, vector<string>> USC2LSmitll;

// 	// if(!firstRUN){
// 	// 	firstRUN = true;
// 	//   const auto mainConfig  = toml::parse(configFile);
// 	//   USC2LSmitll = toml::get<map<string, vector<string>>>(mainConfig.at("USC2LSmitllTable"));
// 	// }

// 	// auto USC2LSmitll_it = USC2LSmitll.find(compName);

// 	// if(USC2LSmitll_it == USC2LSmitll.end()){			// remove for speed increase
// 	// 	cout << "Key not found: \"" << USC2LSmitll_it->first << "\"" << endl;
// 	// 	return "\0";
// 	// }

// 	// for(unsigned int i = 0; i < USC2LSmitll_it->second.size(); i++){
// 	// 	if(USC2LSmitll_it->second[i] == pinName){
// 	// 		return USC2LSmitll_it->second[i];
// 	// 	}
// 	// }

// 	// cout << "pin: \"" << pinName << "\" NOT FOUND in \"" <<  compName << "\""<< endl;
// 	// return "\0";
// }

// /**
//  * [ptlStats - Calculates the stats of the route/PTLs in DEF file]
//  * @param    defFileName [File name of the DEF file to be read]
//  * @return	   					 [1 - All good; 0 - error]
//  */

// int ptlStats(string defFileName){
// 	// vector<def_net> def_cl_nets;
// 	// // defimport(defFileName); // already called
// 	// getNetVec(def_cl_nets);

// 	// unsigned int lenMax = 0;				// Maximum length of the PTLs
// 	// unsigned int lenMin = 10000;		// Minimum length of the PTLs
// 	// float lenMean = 0;							// The mean/average length of the PTLs
// 	// unsigned int lenCnt = 0;				// Number of PTLs
// 	// unsigned long lenSum = 0;				// The sum of the PTL length
// 	// float stdVar = 0; 							// Standards variance


// 	// cout << "Calculating stats on the transmission lines." << endl;
// 	// lenCnt = def_cl_nets.size();
// 	// for(unsigned int i = 0; i < lenCnt; i++){
// 	//   lenSum += def_cl_nets[i].get_trans_delay();

// 	//   if(def_cl_nets[i].get_trans_delay() > lenMax)
// 	//   	lenMax = def_cl_nets[i].get_trans_delay();

// 	//   if(def_cl_nets[i].get_trans_delay() < lenMin)
// 	//   	lenMin = def_cl_nets[i].get_trans_delay();
// 	// }

// 	// lenMean = (float)lenSum / (float)lenCnt;

// 	// for(unsigned int i = 0; i < lenCnt; i++){
// 	//   stdVar += pow((float)def_cl_nets[i].get_trans_delay() - lenMean, 2);
// 	// }

// 	// stdVar /= lenCnt;

// 	// stdVar = sqrt(stdVar);

// 	// cout << "PTLs:" << endl;
// 	// cout << "\tCnt: "  << lenCnt << endl;
// 	// cout << "\tMin: "  << lenMin << endl;
// 	// cout << "\tMean: "  << lenMean << endl;
// 	// cout << "\tMax: "  << lenMax << endl;
// 	// cout << "\tTotal: "  << lenSum << endl;
// 	// cout << "\tstdVar: "  << stdVar << endl;

// 	return 1;
// }