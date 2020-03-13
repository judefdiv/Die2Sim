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
// #include <map>

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

	set<string> validCommands = {"-j", "-d", "-v", "-h"};

	string outFName = "\0";			// The output file, which is follow by the -o parameter
	string defFName = "\0";			// The DEF file
	string command  = "\0";			// The command to be executed

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

	// // search for .lef
	// for(int i = 0; i < argCount; i++){
	// 	foo = string(argValues[i]);
	//   if(foo.find(".lef")!=string::npos){
	//   	lefFName = foo;
	//   }
	// }

	// search for .def
	for(int i = 0; i < argCount; i++){
		foo = string(argValues[i]);
	  if(foo.find(".def")!=string::npos){
	  	defFName = foo;
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

void helpScreen(){
	cout << "===============================================================================" << endl;
	cout << "Usage: Die2Sim [ OPTION ] [ filenames ]" << endl;
	cout << "-j(oSIM)      Converts LEF/DEF to .cir then simulates it through JoSIM." << endl;
	cout << "                [.def file] -o [.cir file]" << endl;
	cout << "-d(ot)        Converts LEF/DEF to a tree diagram." << endl;
	cout << "                [.def file] -o [.jpg file]" << endl;
	cout << "-c(onfig)     Runs the tools using the parameters in the config.toml file." << endl;
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