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
	defJosim.genCir(cirFileName);

	return 1;
}


/**
 * [def2josim::fetchData - gets the data from the appropriate places]
 * @param  ConfigFileName [The config.toml file]
 * @param  DefFileName    [The .def file name]
 * @return                [0 - Exit Success; 1 - Exit Failure]
 */

int def2josim::fetchData(string ConfigFileName, string DefFileName){

	const auto mainConfig      = toml::parse(ConfigFileName);
	this->NetListLoc           = toml::get<unordered_map<string, string>>(mainConfig.at("NETLIST_LOCATIONS"));
	this->USC2LSmitllMap       = toml::get<unordered_map<string, string>>(mainConfig.at("USC2LSmitll"));
	this->NetListPins          = toml::get<unordered_map<string, vector<string>>>(mainConfig.at("LSmitll_netlist"));

	const auto& tomlparameters = toml::find(mainConfig, "Parameters");
	para_PTL_length            = toml::find<float>(tomlparameters, "fix_PTL_length");
	para_mergeIntoSubcir			 = toml::find<bool>(tomlparameters, "merge_into_subcircuit");

  // Creates a map with the number of pins of each sub-circuit
	unordered_map<string, vector<string>>::iterator it;
	for(it = this->NetListPins.begin(); it != this->NetListPins.end(); it++){
	  this->NetListPinNo.insert(pair<string,int>(it->first, it->second.size()));
	}


	/**
	 * Should be a better place to put this...
	 */


	def_file defFileIn;
	defFileIn.importFile(DefFileName);

	defFileIn.PTLstats();

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
	joFile.setMergeIntoSubcir(para_mergeIntoSubcir);

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
		numberpins = this->subcktNoPins(USC2LSmitll(comp.getCompType()));

		// net.clear();
		net.resize(numberpins);

		for(int j = 0; j < numberpins; j++){
			net[j] = "????";
		}

		tempCompNet.compType = comp.getCompType();
		tempCompNet.nets = net;
		nets[comp.getName()] = tempCompNet;
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
			// index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitll(itNet.get_varFromPin()));
			index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitllPin(nets_it->second.compType, itNet.get_varFromPin()));
	    nets_it->second.nets[index] = to_string(i) + "A";

	  } else{
	  	cout << "Component \"" << itNet.get_varFromComp() << "\" not found." << endl;
	  }

		// must join
		nets_it = nets.find(itNet.get_varToComp());
		if(nets_it != nets.end()){
			// index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitll(itNet.get_varToPin()));
			index = subcktPinLoc(USC2LSmitll(nets_it->second.compType), USC2LSmitllPin(nets_it->second.compType, itNet.get_varToPin()));
	    nets_it->second.nets[index] = to_string(i) + "B";

	  } else{
	  	cout << "Component \"" << itNet.get_varToComp() << "\" not found." << endl;
	  }

	  if(this->para_PTL_length == -1){
	  	joFile.pushPTL(itNet.get_varName(), to_string(i), itNet.get_track_length());
	  }
	  else {
	  	joFile.pushPTL(itNet.get_varName(), to_string(i), para_PTL_length);
	  }

	  i++;
	}

	cout << "Adding component to JoSIM class" << endl;

	for(auto &[compName, data]: nets){
		joFile.pushComp(compName, USC2LSmitll(data.compType), data.nets);
	}

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
/**
 * TO BE REMOVED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

string def2josim::USC2LSmitllPin(string compName, string pinName){
	// cout << "Looking for comp: " << compName << "  pin: " << pinName << endl;
	if(!USC2LSmitll(compName).compare("LSmitll_SPLITT") && !pinName.compare("IN_CLK")){
		return "a";
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

	if(cellName == "LSmitll_SPLITT" && pinName == "IN_CLK"){
		pinName = "a";
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
