/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-03-30
 * Modified:
 * license:
 * Description: Class for storing .def
 * File:				ClassDef.cpp
 */

#include "die2sim/ClassDef.hpp"

/**
 * DEF file class functions
 */

void def_file::createAuto(vector<string> &inLine){
	string keyword = inLine[0];

	if(keyword == "DIEAREA"){
		if(inLine.size() != 11){
			cout << "DIEAREA syntax has changed, break..." << endl;
		}
		this->DIEAREA[0] = stoi(inLine[2]);
		this->DIEAREA[1] = stoi(inLine[3]);
		this->DIEAREA[2] = stoi(inLine[6]);
		this->DIEAREA[3] = stoi(inLine[7]);
	}
	else if(keyword == "UNITS"){
		if(inLine[1] == "DISTANCE"){
			this->unitsUnits = inLine[2];
			this->unitsVar = stoi(inLine[3]);
		}
	}
	else if(keyword == "DESIGN"){
		this->DESIGN = inLine[1];
	}
	else{
		cout << "Skipping line." << endl;
	}
}

/**
 * [def_file::importFile description]
 * @param  fileName [File name of the def file to be imported]
 * @return          [1 - All good, 0 - Error]
 */

int def_file::importFile(string fileName){
	vector<string> lineVec;
	vector<vector<string> > strBlock;
	string keyword;

	ifstream defFile;

	bool proOH = false;

	cout << "Importing DEF file \""  << fileName << "\"" << endl;

	defFile.open(fileName);

	if(!defFile.is_open()){
		cout << "DEF file \""  << fileName << "\" failed to be opened." << endl;
		return 0;
	}

	while(1){
		lineVec = splitFileLine(defFile);
		keyword = lineVec[0];
		// disVector(lineVec);

		if(this->validNBlkWords.find(keyword) != this->validNBlkWords.end()){
			if(keyword == "COMPONENTS"){
				cout << "Processing components..." << endl;
				this->comps.resize(stoi(lineVec[1]));

				unsigned int compIndex = 0;
				lineVec = splitFileLine(defFile);
				while(lineVec[0] != "END" && lineVec[1] != "COMPONENTS"){
					this->comps[compIndex++].createAuto(lineVec);
					lineVec = splitFileLine(defFile);
				}
			}
			else if(keyword == "NETS"){
				cout << "Processing nets..." << endl;
				this->nets.resize(stoi(lineVec[1]));

				unsigned int netIndex = 0;
				lineVec = splitFileLine(defFile);

				while(lineVec[0] != "END" && lineVec[1] != "NETS"){
					while(lineVec[lineVec.size()-1] != ";"){
						strBlock.push_back(lineVec);
						lineVec = splitFileLine(defFile);
					}
					strBlock.push_back(lineVec);

					// disVectorBlk(strBlock);
					this->nets[netIndex++].createAuto(strBlock);

					strBlock.clear();
					lineVec = splitFileLine(defFile);
				}
			}
			else if(keyword == "SPECIALNETS"){
				cout << "Processing special nets..." << endl;
				this->snets.resize(stoi(lineVec[1]));

				unsigned int snetIndex = 0;
				lineVec = splitFileLine(defFile);

				while(lineVec[0] != "END" && lineVec[1] != "SPECIALNETS"){
					while(lineVec[lineVec.size()-1] != ";"){
						strBlock.push_back(lineVec);
						lineVec = splitFileLine(defFile);
					}
					strBlock.push_back(lineVec);

					// disVectorBlk(strBlock);
					this->snets[snetIndex++].createAutoSpecial(strBlock);

					strBlock.clear();
					lineVec = splitFileLine(defFile);
				}
			}
			else{
				cout << "Check for smoke." << endl;
			}
		}
		else if(validOHWords.find(keyword) != validOHWords.end()){
			if(!proOH){
				cout << "Processing overheads..." << endl;
				proOH = true;
			}
			this->createAuto(lineVec);
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

vector<string> splitFileLine(ifstream &inFile){
	string lineIn;
	char frontChar;

	static vector<string> errorVec = {"\0"};

	while(getline(inFile, lineIn)){
		frontChar = (char)lineIn.front();
		if(frontChar == '\n' || frontChar == '#' || frontChar == '\0'){			// skips commented and empty lines
			//just loop
		}
		else{
			return SplitStrVec(lineIn);
		}
	}
	cout << "Extracting data file file error!!!" << endl;
	return errorVec;
}

/**
 * [def_file::to_str - displays the class data]
 */

void def_file::to_str(){
	cout << "DEF file Class:" << endl;
	cout << "\tDesign: " << this->DESIGN << endl;
	cout << "\tUnits: " << this->unitsUnits << "  " << this->unitsVar << endl;
	cout << "\tDie area(x,y): " << this->DIEAREA[0] << "  " << this->DIEAREA[1] << "  by  " << this->DIEAREA[2] << "  " << this->DIEAREA[3] << endl;

	for(unsigned int i = 0; i < this->comps.size(); i++){
		this->comps[i].to_str();
	}

	for(unsigned int i = 0; i < this->nets.size(); i++){
		this->nets[i].to_str();
	}

	for(unsigned int i = 0; i < this->snets.size(); i++){
		this->snets[i].to_str();
	}
}

/**
 * Components class functions
 */

int def_component::createAuto(vector<string> &inLine){
	this->name = inLine[1];
	this->compName = inLine[2];
	this->posType = inLine[4];
	this->pt[0] = (int)stod(inLine[6]);
	this->pt[1] = (int)stod(inLine[7]);
	this->orient = inLine[9];

	return 1;
}

void def_component::to_str(){
	cout << "Component Class:" << endl;
	cout << "\tName: " << this->name << endl;
	cout << "\tComponent: " << this->compName << endl;
	cout << "\tPosition type: " << this->posType << endl;
	cout << "\tPosition(x,y): " << this->pt[0] << "  " << this->pt[1] << endl;
	cout << "\t Orientation: " << this->orient << endl;
}

/**
 * Net class functions
 */

int def_net::createAuto(vector<vector<string> > &inBlock){
	this->name = inBlock[0][1];
	this->fromComp = inBlock[1][1];
	this->fromPin = inBlock[1][2];
	this->ToComp = inBlock[2][1];
	this->ToPin = inBlock[2][2];

	for(int i = 3; i < inBlock.size(); i++){
		net_route tempRoute;
		string tempChar;

		int k = 1;

		tempChar = inBlock[i][0];
		if(tempChar == "+"){
			k++;
		}

		tempRoute.LAYER = inBlock[i][k++];

		while(inBlock[i][k] == "("){
			if(inBlock[i][++k] != "*")
				tempRoute.ptX.push_back((int)stod(inBlock[i][k]));
			else{
				int j = 4;
				while(inBlock[i][k-j] == "*"){
					j = j + 4;
				}
				tempRoute.ptX.push_back((int)stod(inBlock[i][k-j]));
			}

			if(inBlock[i][++k] != "*")
				tempRoute.ptY.push_back((int)stod(inBlock[i][k]));
			else{
				int j = 4;
				while(inBlock[i][k-j] == "*"){
					j = j + 4;
				}
				tempRoute.ptY.push_back((int)stod(inBlock[i][k-j]));
			}

			k = k + 2;
			if(k < inBlock[i].size()){
				tempChar = inBlock[i][k];
				if(tempChar != "(" && tempChar != ";" && tempChar != "\0"){
					tempRoute.VIA = inBlock[i][k];
					break;
				}
			}
		}
		this->routes.push_back(tempRoute);
	}
	return 1;
}

/**
 * [def_net::get_trans_delay - Calculates the length of the track]
 * @return [Returns the distance in nano meters]
 */

float def_net::get_trans_delay(){
	unsigned long long trackLen = 0;

	for(unsigned int i = 0; i < this->routes.size(); i++){

		for(unsigned int j = 1; j < this->routes[i].ptX.size(); j++){
			if(routes[i].ptX.size() == 1){		// can be omitted
				// Skips single point/dimension tracks, can be due to via placement
				continue;
			}

			trackLen += abs(this->routes[i].ptX[j] - this->routes[i].ptX[j-1]) *10;
			trackLen += abs(this->routes[i].ptY[j] - this->routes[i].ptY[j-1]) *10;
		}
	}

	return trackLen /10000;
}

void def_net::to_str(){
	cout << "Net Class:" << endl;
	cout << "\tName: " << this->name << endl;

	cout << "\tOrigin:" << endl;
	cout << "\t\tComponent:" << this->fromComp << endl;
	cout << "\t\tPin:" << this->fromPin << endl;

	cout << "\tMustJoin:" << endl;
	cout << "\t\tComponent:" << this->ToComp << endl;
	cout << "\t\tPin:" << this->ToPin << endl;

	for(int i = 0; i < this->routes.size(); i++){
		cout << "\tRoute:" << endl;
		cout << "\t\tLayer: " << this->routes[i].LAYER << endl;
		cout << "\t\tVIA: " << this->routes[i].VIA << endl;
		cout << "\t\tPoints(x,y):" << endl;
		for(int j = 0; j < this->routes[i].ptX.size(); j++){
			cout << "\t\t\t" << this->routes[i].ptX[j];
			cout << "\t" << this->routes[i].ptY[j] << endl;
		}
	}
}

/**
 * Specialnet class functions
 */

 int def_net::createAutoSpecial(vector<vector<string> > &inBlock){
	this->name = inBlock[0][1];

	for(int i = 1; i < inBlock.size(); i++){
		net_route tempRoute;
		string tempChar;
		int k;

		k = 1;

		tempChar = inBlock[i][0];
		if(tempChar == "+"){
			k++;
		}

		tempRoute.LAYER = inBlock[i][k++];
		tempRoute.trackWidth = stoi(inBlock[i][k++]);

		disVector(inBlock[i]);

		while(inBlock[i][k] == "("){
			if(inBlock[i][++k] != "*")
				tempRoute.ptX.push_back((int)stod(inBlock[i][k]));
			else{
				int j = 4;
				while(inBlock[i][k-j] == "*"){
					j = j + 4;
				}
				tempRoute.ptX.push_back((int)stod(inBlock[i][k-j]));
			}

			if(inBlock[i][++k] != "*")
				tempRoute.ptY.push_back((int)stod(inBlock[i][k]));
			else{
				int j = 4;
				while(inBlock[i][k-j] == "*"){
					j = j + 4;
				}
				tempRoute.ptY.push_back((int)stod(inBlock[i][k-j]));
			}

			k = k + 2;

			unsigned int fooSize;
			fooSize = inBlock[i].size();

			if(k >= fooSize){
				k = inBlock[i].size() - 1;
			}
			// unsigned int fooSize;
			// fooSize = inBlock[i].size();

			// if(k < fooSize){
			// 	tempChar = inBlock[i][k];
			// 	if(tempChar != "(" && tempChar != ";" && tempChar != "\0"){
			// 		tempRoute.VIA = inBlock[i][k];
			// 		break;
			// 	}
			// }
		}
		this->routes.push_back(tempRoute);

	}
	return 1;
}