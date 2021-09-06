#include "die2sim/ToVerilog.hpp"
#include "die2sim/ParserDef.hpp"
#include "die2sim/genFunc.hpp"
#include "toml/toml.hpp"
#include <sstream>

using namespace verilog;
using namespace verilogemit;

void def2verilog::dummyGenV(){
	vf = verilogemit::VerilogFile("1ps/100fs", "HalfAdder");
	for(int i=0; i<10; i++){
		vf.addPtl({i, (float)(i*2)});
	}
	int ptlid = 0;
	for(int i=0; i<10; i++){
		vector<Port> ports;
		if(i%2 == 0){
			ports.push_back({"a", IN, ptlid++});
			ports.push_back({"clk", CLK, ptlid++});
			ports.push_back({"out", OUT, ptlid++});
			vf.addInclude("modules/LSmitll_MyCell1.v");
			vf.addCell({"MyCell1", to_string(i), ports});
		}else{
			ports.push_back({"a", IN, ptlid++});
			ports.push_back({"b", IN, ptlid++});
			ports.push_back({"clk", CLK, ptlid++});
			ports.push_back({"out", OUT, ptlid++});
			vf.addInclude("modules/LSmitll_MyCell2.v");
			vf.addCell({"MyCell2", to_string(i), ports});
		}
	}
	vector<Port> ports = 	{	{"a", IN,  0}, 
								{"b", IN,  1},
								{"clk", CLK, 2},
								{"s", OUT, 3},
								{"cout", OUT, 4}
							};
	for (Port port : ports){
		vf.addPort(port);
	}
	vf.emit("mymodule.v");
}

void def2verilog::genV(){
	dummyGenV();
}

int def2verilog::fetchData(string ConfigFileName, string DefFileName){
    const auto mainConfig       = toml::parse(ConfigFileName);
	NetListLoc                  = toml::get<unordered_map<string, string>>(mainConfig.at("NETLIST_LOCATIONS"));
	USC2LSmitllMap              = toml::get<unordered_map<string, string>>(mainConfig.at("TRANSLATION_TABLE"));
	NetListPins                 = toml::get<unordered_map<string, vector<string>>>(mainConfig.at("GATE_NETLIST"));

	const auto& tomlparameters  = toml::find(mainConfig, "PARAMETERS");
	para_dangling_net           = toml::find<string>(tomlparameters, "dangling_net_name");
	para_PTL_length_bool        = toml::find<bool>(tomlparameters, "force_PTL_Length");
	para_PTL_length             = toml::find<float>(tomlparameters, "force_PTL_length_value");
	input_keys                  = toml::find<vector<string>>(tomlparameters, "input_name_keys");
	output_keys                 = toml::find<vector<string>>(tomlparameters, "output_names_keys");
	clock_keys                  = toml::find<vector<string>>(tomlparameters, "clock_names_keys");

    // Creates a map with the number of pins of each sub-circuit
	unordered_map<string, vector<string>>::iterator it;
	for(it = NetListPins.begin(); it != NetListPins.end(); it++){
	  NetListPinNo.insert(pair<string,int>(it->first, it->second.size()));
	}

	def_file defFileIn;
	defFileIn.importFile(DefFileName);

	defComps = defFileIn.getComps();
	defNets = defFileIn.getNets();

	return 0;
}

int verilog::executeDef2Verilog(string ConfigFileName, string DefFileName, string vFilename){
    cout << "Compiling LEF/DEF for Verilog" << endl;
	def2verilog def2Verilog;
	def2Verilog.fetchData(ConfigFileName, DefFileName);
    def2Verilog.genV();
    return 0;
}


