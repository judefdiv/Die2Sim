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
 * OverHeads class functions
 */

def_OverHeads::def_OverHeads(){

}

void def_OverHeads::createAuto(vector<string> &inLine){
	string keyword = inLine[0];
	if(keyword == "ROW"){
		if(inLine.size() != 14){
			cout << "USC changed syntax, break..." << endl;
		}
		OH_row tempROW;
		tempROW.siteName = inLine[1];
		tempROW.origX = stoi(inLine[2]);
		tempROW.origY = stoi(inLine[3]);
		tempROW.numX = stoi(inLine[6]);
		tempROW.numY = stoi(inLine[8]);
		tempROW.stepX = stoi(inLine[10]);
		tempROW.stepY = stoi(inLine[11]);

		this->ROW.push_back(tempROW);
	}
	else if(keyword == "TRACKS"){
		OH_tracks tempTRACK;
		tempTRACK.axis = inLine[1];
		tempTRACK.start = stoi(inLine[2]);
		tempTRACK.numtracks = stoi(inLine[4]);
		tempTRACK.space = stoi(inLine[6]);
		tempTRACK.LAYER = inLine[8];

		this->TRACKS.push_back(tempTRACK);
	}
	else if(keyword == "GCELLGRID"){
		if(inLine[1] == "X"){
			this->GCGx[0] = stoi(inLine[2]);
			this->GCGx[1] = stoi(inLine[4]);
			this->GCGx[2] = stoi(inLine[6]);
		}
		else if(inLine[1] == "Y"){
			this->GCGy[0] = stoi(inLine[2]);
			this->GCGy[1] = stoi(inLine[4]);
			this->GCGy[2] = stoi(inLine[6]);
		}
		else
			cout << "GCELLGRID syntax error" << endl;
	}
	else if(keyword == "DIEAREA"){
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
		cout << "Uhm, this is not suppose to be here." << endl;
	}
}

void def_OverHeads::to_str(){
	cout << "Overheads Class:" << endl;
	cout << "\tDesign: " << this->DESIGN << endl;
	cout << "\tUnits: " << this->unitsUnits << "  " << this->unitsVar << endl;
	cout << "\tDie area(x,y): " << this->DIEAREA[0] << "  " << this->DIEAREA[1] << "  by  " << this->DIEAREA[2] << "  " << this->DIEAREA[3] << endl;

	cout << "\tGCellGrid:" << endl;
	cout << "\t\tX: start: " << this->GCGx[0] << "  count: " << this->GCGx[1] << "  step: " << this->GCGx[2] << endl;
	cout << "\t\tY: start: " << this->GCGy[0] << "  count: " << this->GCGy[1] << "  step: " << this->GCGy[2] << endl;


	cout << "\tROW: " << endl;
	if(this->ROW.size() > 0){
		for(int i = 0; i < this->ROW.size(); i++){
			cout << "\t\tName: " << this->ROW[i].name << endl;
			cout << "\t\tSite name: " << this->ROW[i].siteName << endl;
			cout << "\t\tOrigin(x,y): " << this->ROW[i].origX << "  " << this->ROW[i].origY << endl;
			cout << "\t\tNumber(x,y): " << this->ROW[i].numX << "  " << this->ROW[i].numY << endl;
			cout << "\t\tStep(x,y): " << this->ROW[i].stepX << "  " << this->ROW[i].stepY << endl;
		}
	}

	cout << "\tTRACKS: " << endl;
	if(this->TRACKS.size() > 0){
		for(int i = 0; i < this->TRACKS.size(); i++){
			cout << "\t\tAxis: " << this->TRACKS[i].axis << endl;
			cout << "\t\tStart: " << this->TRACKS[i].start << endl;
			cout << "\t\tNumber: " << this->TRACKS[i].numtracks << endl;
			cout << "\t\tSpace: " << this->TRACKS[i].space << endl;
			cout << "\t\tLayer: " << this->TRACKS[i].LAYER << endl;
		}
	}

}

/** 
 * Components class functions
 */

int def_component::compCnt = 0;

def_component::def_component(vector<string> &inLine){
	this->classID = this->compCnt++;
	this->name = inLine[1];
	this->compName = inLine[2];
	this->posType = inLine[4];
	// this->pt[0] = stoi(inLine[6]);
	// this->pt[1] = stoi(inLine[7]);
	this->pt[0] = (int)stod(inLine[6]);
	this->pt[1] = (int)stod(inLine[7]);
	this->orient = inLine[9];
}

void def_component::to_str(){
	cout << "Component Class:" << endl;
	cout << "\tName: " << this->name << endl;
	cout << "\tComponent: " << this->compName << endl;
	cout << "\tIDno: " << this->classID << endl;
	cout << "\tPosition type: " << this->posType << endl;
	cout << "\tPosition(x,y): " << this->pt[0] << "  " << this->pt[1] << endl;
	cout << "\t Orientation: " << this->orient << endl;
}

/** 
 * Net class functions
 */

int def_net::netCnt = 0;

def_net::def_net(vector<vector<string> > &inBlock){
	this->classID = this->netCnt++;
	this->name = inBlock[0][1];
	this->fromComp = inBlock[1][1];
	this->fromPin = inBlock[1][2];
	this->ToComp = inBlock[2][1];
	this->ToPin = inBlock[2][2];

	for(int i = 3; i < inBlock.size(); i++){
		net_route tempRoute;

		string tempChar = inBlock[i][0];
		while(tempChar == "+" || tempChar == "ROUTED" || tempChar == "NEW"){
			inBlock[i].erase(inBlock[i].begin());
			tempChar = inBlock[i][0];
		}

		tempRoute.LAYER = inBlock[i][0];

		int k = 1;
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
			tempChar = inBlock[i][k];
			if(tempChar != "(" && tempChar != ";" && tempChar != "\0"){
				tempRoute.VIA = inBlock[i][k];
				break;
			}
		}
		this->routes.push_back(tempRoute);
		
	}

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
	cout << "\tIDno: " << this->classID << endl;

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

int def_specialNet::netCnt = 0;

def_specialNet::def_specialNet(vector<vector<string> > &inBlock){
	this->classID = this->netCnt++;
	this->name = inBlock[0][1];

	for(int i = 1; i < inBlock.size(); i++){
		net_route tempRoute;

		string tempChar = inBlock[i][0];
		while(tempChar == "+" || tempChar == "ROUTED" || tempChar == "NEW"){
			inBlock[i].erase(inBlock[i].begin());
			tempChar = inBlock[i][0];
		}

		tempRoute.LAYER = inBlock[i][0];
		tempRoute.trackWidth = stoi(inBlock[i][1]);

		int k = 2;
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
			tempChar = inBlock[i][k];
			if(tempChar != "(" && tempChar != ";" && tempChar != "\0"){
				tempRoute.VIA = inBlock[i][k];
				break;
			}
		}
		this->routes.push_back(tempRoute);
		
	}

}

void def_specialNet::to_str(){
	cout << "SNet Class:" << endl;
	cout << "\tName: " << this->name << endl;
	cout << "\tIDno: " << this->classID << endl;

	for(int i = 0; i < this->routes.size(); i++){
		cout << "\tRoute:" << endl;
		cout << "\t\tLayer: " << this->routes[i].LAYER << endl;
		cout << "\t\tVIA: " << this->routes[i].VIA << endl;
		cout << "\t\tWidth: " << this->routes[i].trackWidth << endl;
		cout << "\t\tPoints(x,y):" << endl;
		for(int j = 0; j < this->routes[i].ptX.size(); j++){
			cout << "\t\t\t" << this->routes[i].ptX[j];
			cout << "\t" << this->routes[i].ptY[j] << endl;
		}
	}
}

// void def_specialNet::get_route(vector<net_route> &VecOut){
// 	VecOut = this->routes;
// }