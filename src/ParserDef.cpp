/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-03-20
 * Modified:
 * license: 
 * Description: Parser for .def
 * File:				ParserDef.cpp
 */

#include "die2sim/ParserDef.hpp"

set<string> validOHWords = {"DESIGN", "UNITS", "DIEAREA", "ROW", "TRACKS", "GCELLGRID"};
set<string> validNBlkWords = {"COMPONENTS", "SPECIALNETS", "NETS"};
ifstream defFile;
vector<def_component> def_cl_comps;
vector<def_net> def_cl_nets;
vector<def_specialNet> def_cl_snets;
def_OverHeads def_OHs;

int defimport(string defFileName){
	vector<string> lineVec;
	vector<vector<string> > strBlock;
	string keyword;
	bool proOH = false;

	cout << "Importing DEF file -> "  << defFileName << endl; 

	defFile.open(defFileName);

	if(!defFile.is_open()){
		cout << "DEF file FAILED to be properly opened" << endl;
		return 0;
	}

	while(proNewLineDef(lineVec)){
		keyword = lineVec[0];
		// disVector(lineVec);
		if(validNBlkWords.find(keyword) != validNBlkWords.end()){
			if(keyword == "COMPONENTS"){
				cout << "Processing components..." << endl; 
				def_cl_comps.reserve(stoi(lineVec[1])); //set size of the vector, may improve performance?
				proNewLineDef(lineVec);
				while(lineVec[0] != "END"){
					def_component tempComp(lineVec);
					def_cl_comps.push_back(tempComp);
					// tempComp.to_str();
					proNewLineDef(lineVec);
				}
			}
			else if(keyword == "SPECIALNETS"){
				cout << "Processing special nets..." << endl; 
				def_cl_snets.reserve(stoi(lineVec[1])); //set size of the vector, may improve performance?
				

				proNewLineDef(lineVec);
				while(lineVec[0] != "END" && lineVec[1] != "SPECIALNETS"){ 
					while(lineVec[lineVec.size()-2] != ";"){
						strBlock.push_back(lineVec);
						proNewLineDef(lineVec);
					}
					strBlock.push_back(lineVec);
					// disVectorBlk(strBlock);
					if(strBlock[1][0] == ";"){
						proNewLineDef(lineVec);
						strBlock.clear();
						continue;
					}
					else{
						def_specialNet tempSnet(strBlock);
						def_cl_snets.push_back(tempSnet);
						// tempSnet.to_str();
						strBlock.clear();
						proNewLineDef(lineVec);
					}
				}

			}
			else if(keyword == "NETS"){
				cout << "Processing nets..." << endl; 
				def_cl_nets.reserve(stoi(lineVec[1])); //set size of the vector, may improve performance?
				

				proNewLineDef(lineVec);
				while(lineVec[0] != "END" && lineVec[1] != "NETS"){ 
					while(lineVec[lineVec.size()-2] != ";"){	
						strBlock.push_back(lineVec);
						proNewLineDef(lineVec);
					}
					strBlock.push_back(lineVec);

					// disVectorBlk(strBlock);
					def_net tempNet(strBlock);
					def_cl_nets.push_back(tempNet);
					// tempNet.to_str();

					strBlock.clear();
					proNewLineDef(lineVec);
				}
			}
			else
				cout << "Check for smoke." << endl;
		}
		else if(validOHWords.find(keyword) != validOHWords.end()){
			if(!proOH){
				cout << "Processing overheads..." << endl; 
				proOH = true;
			}
			def_OHs.createAuto(lineVec);
		}
		else if(lineVec[0] == "END" && lineVec[1] == "DESIGN"){
			cout << "Importing DEF file done." << endl;
			break;
		}
		else{
			cout << "Unknown word." << endl;
			return 0;
		}			
	}

	defFile.close();
	return 1;
}

/**
 * [disDEFdata - Displays all the .to_str of all the classes]
 */

void disDEFdata(){
	// Overheads
	// def_OHs.to_str();

	// Components
	// for(unsigned int i = 0; i < def_cl_comps.size(); i++) def_cl_comps[i].to_str();

	// Nets
	for(unsigned int i = 0; i < def_cl_nets.size(); i++) def_cl_nets[i].to_str();

	// Special Nets
	// for(unsigned int i = 0; i < def_cl_snets.size(); i++) def_cl_snets[i].to_str();
}


int proNewLineDef(vector<string> &inVec){
	string lineIn;
	char frontChar;

	while(getline(defFile, lineIn)){
		frontChar = (char)lineIn.front();
		if(frontChar == '\n' || frontChar == '#' || frontChar == '\0'){			// skips commented and empty lines
			//just loop
		}
		else{
			stringSplitVec(inVec, lineIn);
			return 1;
		}
	}
	return 0;
}

void getNetVec(vector<def_net> &outVec){
	outVec = def_cl_nets;
}

void getSNetsVec(vector<def_specialNet> &outVec){
	outVec = def_cl_snets;
}

void getCompVec(vector<def_component> &outVec){
	outVec = def_cl_comps;
}