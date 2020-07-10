/**
 * Author:  		JF de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-03-20
 * Modified:
 * license:
 * Description: Primary file for the program.
 * File:				main.cpp
 */

#include <iostream>				//stream
#include <string>					//string goodies
#include <set>
#include <map>

#include "die2sim/genFunc.hpp"
#include "toml/toml.hpp"
#include "die2sim/ToJosim.hpp"

#define versionNo 0.9
#define parameterFile "config.toml"

using namespace std;

/**
 * Declaring functions
 */

void welcomeScreen();
void helpScreen();
int runFromFile(const string &fileName);
int RunTool(int argCount, char** argValues);

/**
 * Main loop
 */

int main(int argc, char* argv[]){
	// welcomeScreen();
	if(!RunTool(argc, argv)) return 0;

	return 0;
}

/**
 * [RunTool - Runs the appropriate tool]
 * @param  argCount  [Size of the input argument array]
 * @param  argValues [The input argument array]
 * @return           [1 - all good; 0 - error]
 */

int RunTool(int argCount, char** argValues){
	welcomeScreen();

	if(argCount <= 1){
		return 0;
	}

	set<string> validCommands = {"-j", "-d", "-v", "-h", "-c"};

	string outFName = "\0";			// The output file, which is follow by the -o parameter
	string defFName = "\0";			// The DEF file
	string command  = "\0";			// The command to be executed
	string tomlFName = "\0";		// config file
	string foo;

	// --------------------------------------------------------------------
	// ---------------------- Searching for parameters --------------------
	// --------------------------------------------------------------------

	// search for command
	for(int i = 0; i < argCount; i++){
		foo = string(argValues[i]);
		if(validCommands.find(foo) != validCommands.end()){
			command = foo;
		}
	}
	if(!command.compare("\0")){
		cout << "Invalid command." << endl;
		return 0;
	}

	// search for .def
	for(int i = 0; i < argCount; i++){
		foo = string(argValues[i]);
	  if(foo.find(".def")!=string::npos){
	  	defFName = foo;
	  }
	}

		for(int i = 0; i < argCount; i++){
		foo = string(argValues[i]);
	  if(foo.find(".toml")!=string::npos){
	  	tomlFName = foo;
	  }
	}

	// --------------------------------------------------------------------
	// ------------------------ Output file naming ------------------------
	// --------------------------------------------------------------------

	// search for output filename
	for(int i = 0; i < argCount-1; i++){
	  if(!string(argValues[i]).compare("-o")){
	  	outFName = string(argValues[i+1]);
	  }
	}
	// auto assign output filename if non has been set
	if(!outFName.compare("\0") && defFName.compare("\0")){
		if(!command.compare("-j")){
			outFName = fileExtensionRenamer(defFName, ".cir");;
		}
	}

	// --------------------------------------------------------------------
	// ------------------------------ Commands ----------------------------
	// --------------------------------------------------------------------

	if(!command.compare("-j")){
		if(defFName.compare("\0") && outFName.compare("\0")){
			executeDef2Josim(parameterFile, defFName, outFName);
			return 1;
		}
		else {
			cout << "Input argument error." << endl;
			return 0;
		}
	}
	else if(!command.compare("-c")){
		if(tomlFName.compare("\0")){
			return runFromFile(tomlFName);
		}
		else {
			cout << "Input argument error." << endl;
			return 0;
		}
	}
	else if(!command.compare("-d")){
		if(defFName.compare("\0") && outFName.compare("\0")){

			def_file defFileIn;
			defFileIn.importFile(defFName);
			defFileIn.to_jpg(outFName);

			return 1;
		}
		else {
			cout << "Input argument error." << endl;
			return 0;
		}
	}
	else if(!command.compare("-v")){
		if(argCount == 1 + 1){
			cout << setprecision(2);
			cout << "Version: " << versionNo << endl;
			return 1;
		}
		cout << "Input argument error." << endl;
		return 0;
	}
	else if(!command.compare("-h")){
		helpScreen();
		return 1;
	}
	else{
		cout << "Quickly catch the smoke before it escapes." << endl;
		return 0;
	}

	cout << "I am smelling smoke." << endl;
	return 0;
}

int runFromFile(const string &fileName){
	cout << "Importing execution parameters from config.toml" << endl;

	const auto mainConfig  = toml::parse(fileName);
	map<string, string> run_para = toml::get<map<string, string>>(mainConfig.at("FILE_LOCATIONS"));

	map<string, string>::iterator it_run_para;

	string command  = "toJoSIM";
	string defFName = "\0";
	string outFName = "\0";
	// string lefFName = "\0";			// .toml
	// string workDir  = "";			  //

	// it_run_para = run_para.find("Command");
	// if(it_run_para != run_para.end()){
	// 	command = it_run_para->second;
	// }
	// else{
	// 	cout << "Invalid parameters." << endl;
	// 	return 0;
	// }

	it_run_para = run_para.find("defFileName");
	if(it_run_para != run_para.end()){
		defFName = it_run_para->second;
		// gdsFName = gdsFName.insert(0, workDir);
	}

	it_run_para = run_para.find("cirFileName");
	if(it_run_para != run_para.end()){
		outFName = it_run_para->second;
		// bliFName = bliFName.insert(0, workDir);
	}


	if(!command.compare("toJoSIM")){
		if(outFName.compare("\0") && defFName.compare("\0")){
			return executeDef2Josim(fileName, defFName, outFName);
		}
		else{
			cout << "Input argument error." << endl;
			return 0;
		}
	}
	else{
		cout << "Invalid command." << endl;
		return 0;
	}

	return 0;
}

void helpScreen(){
	cout << "===============================================================================" << endl;
	cout << "Usage: Die2Sim [ OPTION ] [ filenames ]" << endl;
	cout << "-j(oSIM)      Converts LEF/DEF to .cir then simulates it through JoSIM." << endl;
	cout << "                [.def file] -o [.cir file]" << endl;
	// cout << "-d(ot)        Converts LEF/DEF to a tree diagram." << endl;
	// cout << "                [.def file] -o [.jpg file]" << endl;
	cout << "-c(onfig)     Runs the tools using the parameters in the config.toml file." << endl;
	cout << "                [.toml file]" << endl;
	cout << "-v(ersion)    Displays the version number." << endl;
	cout << "-h(elp)       Help screen." << endl;
	cout << "===============================================================================" << endl;
}

/**
 * Welcoming screen
 */

void welcomeScreen(){
	cout << "=====================================" << endl;
	cout << "              Die2Sim" << endl;
	cout << "       Author JF de Villiers" << endl;
	cout << "      Stellenbosch University" << endl;
	cout << "    For IARPA, ColdFlux project" << endl;
	cout << "=====================================" << endl;
}