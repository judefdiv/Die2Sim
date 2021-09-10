#include "die2sim/ToVerilog.hpp"
#include "die2sim/ParserDef.hpp"
#include "die2sim/genFunc.hpp"
#include "toml/toml.hpp"
#include <sstream>

using namespace verilog;
using namespace verilogemit;

void def2verilog::dummyGenV(){
	vf = verilogemit::VerilogFile("1ps/100fs", "HalfAdder");
	for(size_t i=0; i<10; i++){
		vf.addPtl({i, (double)(i*2)});
	}
	size_t ptlid = 0;
	for(size_t i=0; i<10; i++){
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

verilogemit::PortType def2verilog::str2pt(string str){
	for(const auto key : input_keys){
		if(str.find(key) != string::npos) return IN;
	}
	for(const auto key : clock_keys){
		if(str.find(key) != string::npos) return CLK;
	}
	for(const auto key : output_keys){
		if(str.find(key) != string::npos) return OUT;
	}
	return UNKNOWN;
}

void def2verilog::genV(){
	// dummyGenV();
	vf = verilogemit::VerilogFile(timeScale, topModuleName);
	unordered_map<string, def_component> compmap;
	unordered_map<string, unordered_map<string, size_t>> cell_PtlMap;

	for(const auto comp : defComps){
		if (comp.compName != "PAD")
			vf.addInclude(NetListLoc[USC2LSmitllMap[comp.compName]]);
		compmap.insert_or_assign(comp.name, comp);
	}
	size_t ptlid = 0;
	for(auto net : defNets){
		vf.addPtl({ptlid++, net.get_trans_delay()});
		def_component fromComp = compmap[net.fromComp];
		def_component ToComp = compmap[net.ToComp];
		if (fromComp.compName == "PAD"){
			vf.addPort({fromComp.name, str2pt(fromComp.name), ptlid});
		} else {
			cell_PtlMap[fromComp.name][USC2LSmitllMap[net.fromPin]] = ptlid;
		}
		if (ToComp.compName == "PAD"){
			vf.addPort({ToComp.name, str2pt(ToComp.name), ptlid});
		} else {
			cell_PtlMap[ToComp.name][USC2LSmitllMap[net.ToPin]] = ptlid;
		}
	}
	for(const auto comp : defComps){
		if (comp.compName == "PAD") continue;
		Cell cell;
		cell.cellName = comp.name;
		string compName = USC2LSmitllMap[comp.compName];
		cell.moduleName = moduleNames[USC2LSmitllMap[comp.compName]];
		for (auto portname : NetListPins[compName]){
			cell.ports.push_back({portname, str2pt(portname), cell_PtlMap[comp.name][portname]});
		}
		vf.addCell(cell);
	}
	vf.emit("mymodule.v");
	cout << "Done" << endl;
}

string verilog::vFileModuleName(string path){
	ifstream is = ifstream(path);
	string line;
	string modname;
	while(!is.eof()){
		getline(is, line);
		const auto tokvec = split(line, " ");
		for(auto it = tokvec.begin(); it != tokvec.end(); it++){
			if ((*it).compare("module") == 0){
				return *(++it);
			}
		}
	}
	return "notfound";
}

int def2verilog::fetchData(string ConfigFileName, string DefFileName){
    const auto mainConfig       = toml::parse(ConfigFileName);
	NetListLoc                  = toml::get<unordered_map<string, string>>(mainConfig.at("VERILOG_MODULE_LOCATIONS"));
	NetListPins                 = toml::get<unordered_map<string, vector<string>>>(mainConfig.at("GATE_NETLIST"));
	USC2LSmitllMap           	= toml::get<unordered_map<string, string>>(mainConfig.at("TRANSLATION_TABLE"));

	const auto& tomlparameters  = toml::find(mainConfig, "PARAMETERS");
	para_dangling_net           = toml::find<string>(tomlparameters, "dangling_net_name");
	para_PTL_length_bool        = toml::find<bool>(tomlparameters, "force_PTL_Length");
	para_PTL_length             = toml::find<float>(tomlparameters, "force_PTL_length_value");
	input_keys                  = toml::find<vector<string>>(tomlparameters, "input_name_keys");
	output_keys                 = toml::find<vector<string>>(tomlparameters, "output_names_keys");
	clock_keys                  = toml::find<vector<string>>(tomlparameters, "clock_names_keys");
	timeScale					= toml::find<string>(tomlparameters, "timescale");
	topModuleName				= toml::find<string>(tomlparameters, "topModuleName");

	def_file defFileIn;
	defFileIn.importFile(DefFileName);

	defComps = defFileIn.getComps();
	defNets = defFileIn.getNets();

	for(auto kv : NetListLoc){
		if (kv.second.compare("PAD") == 0) continue; 
		moduleNames.insert_or_assign(kv.first, verilog::vFileModuleName(kv.second));
	}

	return 0;
}

int verilog::executeDef2Verilog(string ConfigFileName, string DefFileName, string vFilename){
    cout << "Compiling LEF/DEF for Verilog" << endl;
	def2verilog def2Verilog;
	def2Verilog.fetchData(ConfigFileName, DefFileName);
    def2Verilog.genV();
    return 0;
}


