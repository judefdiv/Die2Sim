/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-03-30
 * Modified:
 * license: 
 * Description: Class for storing .lef
 * File:				ClassLef.cpp
 */

#include "die2sim/ClassLef.hpp"

int no_total_classes = 0;

/** 
 * Unit class functions
 */

int lef_units::no_unit_classes = 0;

lef_units::lef_units(){

}

void lef_units::createAuto(vector<vector<string> > &inBlock){
	unsigned int i = 0;
	lef_units::no_unit_classes++;
	no_total_classes++;
	this->classID = no_total_classes;

	while(i < inBlock.size() -1){
		if(inBlock[i][0] == "DATABASE"){
			this->database = inBlock[i][1];
			this->dbCount = stoi(inBlock[i][2]);
		}
		i++;
	}
}

void lef_units::to_str(){
	cout << "Unit Class:" << endl;
	cout << "\tID no: " << this->classID << endl;
	cout << "\tDatabase:" << this->database << endl;
	cout << "\tDatabase count:" << this->dbCount << endl; 
}


/** 
 * Layer class functions
 */

int lef_layer::no_layer_classes = 0;
int lef_layer::no_layer_routing = 0;
int lef_layer::no_layer_masterS = 0;
int lef_layer::no_layer_overlap = 0;

lef_layer::lef_layer(vector<vector<string> > &inBlock){
	lef_layer::no_layer_classes++;
	no_total_classes++;

	int i = 0;

	while(i < inBlock.size() -1){
		if(inBlock[i][0] == "LAYER"){
			this->name = inBlock[i][1];
			this->classID = no_total_classes;
		}
		else if(inBlock[i][0] == "TYPE"){
			this->TYPE = inBlock[i][1];
			if(this->TYPE == "ROUTING"){
				// stuffs should run happily
				lef_layer::no_layer_routing++;
			}
			else if(this->TYPE == "OVERLAP"){
				lef_layer::no_layer_overlap++;
				this->PITCHx = 0;
				this->PITCHy = 0;
				this->WIDTH = 0;
				this->SPACING = 0;
				this->DIRECTION = "\0";
				//break;	//should reach end of loop
			}
			else if(this->TYPE == "MASTERSLICE"){
				lef_layer::no_layer_masterS++;
				this->PITCHx = 0;
				this->PITCHy = 0;
				this->WIDTH = 0;
				this->SPACING = 0;
				this->DIRECTION = "\0";
				//break;	//should reach end of loop
			}
			else{
				cout << "Layer constructor TYPE error" << endl;
				break;
			}
		}
		else if(inBlock[i][0] == "PITCH"){
			if(inBlock[i].size() == 4){
				this->PITCHx = stof(inBlock[i][1]);
				this->PITCHy = stof(inBlock[i][1]);
			}
			else if(inBlock[i].size() == 5){
				this->PITCHx = stof(inBlock[i][1]);
				this->PITCHy = stof(inBlock[i][2]);
			}
		}
		else if(inBlock[i][0] == "WIDTH"){
			this->WIDTH = stof(inBlock[i][1]);
		}
		else if(inBlock[i][0] == "SPACING"){
			this->SPACING = stof(inBlock[i][1]);
		}
		else if(inBlock[i][0] == "DIRECTION"){
			this->DIRECTION = inBlock[i][1];
		}
		else{
			cout << "Layer constructor parameter error" << endl;
			break;
		}
		i++;
	}
}


void lef_layer::to_str(){
	cout << "Layer Class:" << endl;
	cout << "\tID no: " << this->classID << endl;
	cout << "\tName: " << this->name << endl;
	cout << "\ttype: " << this->TYPE << endl;
	cout << "\tDirection: " << this->DIRECTION << endl;
	cout << "\tPitchX: " << this->PITCHx << endl;
	cout << "\tPitchY: " << this->PITCHy << endl;
	cout << "\tWidth: " << this->WIDTH << endl;
	//cout << "\t: " << this-> << endl;
}

/** 
 * Via class functions
 */

int lef_via::no_via_classes = 0;

lef_via::lef_via(vector<vector<string> > &inBlock){
	int i = 0;
	int layerCount = 0;
	lef_via::no_via_classes++;
	no_total_classes++;
	this->classID = no_total_classes;

	while(i < inBlock.size() -1){
		if(inBlock[i][0] == "VIA"){
			this->name = inBlock[i][1];
			this->classID = no_total_classes;

			inBlock[i][2] == "DEFAULT"? this->DEFAULT = true : this->DEFAULT = false;
			// if(inBlock[i][2] == "DEFAULT")
			// 	this->DEFAULT = true;
			// else
			// 	this->DEFAULT = false;
		}
		else if(inBlock[i][0] == "LAYER"){
			this->LayerNames[layerCount] = inBlock[i][1];
			i++;
			if(inBlock[i][0] == "RECT"  && layerCount < 3){
				for(int k = 0; k < 4; k++){
					this->layerDim[layerCount][k] = stof(inBlock[i][k+1]);
				}
				layerCount++;
			}
			else if(inBlock[i][0] == "POLYGON" && layerCount < 3){
				cout << "Polygon is not supported" << endl;
			}
			else{
				cout << "Parameters are not matching up" << endl;
			}
		}
		i++;
	}

}

void lef_via::to_str(){
	cout << "Via Class:" << endl;
	cout << "\tID no: " << this->classID << endl;
	cout << "\tName: " << this->name << endl;
	cout << "\tDefault: "; this->DEFAULT? cout << "yes\n" : cout << "no\n";
	cout << setprecision(4);
	cout << "\tTop layer:" << this->LayerNames[0] << endl;
	// cout << "\tTop layer type:" << this->topLayerType << endl;
	cout << "\t"; printLineArrayF(this->layerDim[0], 4);
	cout << "\tMiddle layer:" << this->LayerNames[1] << endl;
	// cout << "\tMiddle layer type:" << this->midLayerType << endl;
	cout << "\t"; printLineArrayF(this->layerDim[1], 4);
	cout << "\tBottom layer:" << this->LayerNames[2] << endl;
	// cout << "\tBottom layer type:" << this->botLayerType << endl;
	cout << "\t"; printLineArrayF(this->layerDim[2], 4);
}

/** 
 *  Spacing class functions
 */


int lef_spacing::no_classes = 0;

lef_spacing::lef_spacing(){

}

void lef_spacing::createAuto(vector<vector<string> > &inBlock){
	unsigned int i = 0;
	lef_spacing::no_classes++;
	no_total_classes++;
	this->classID = no_total_classes;

	while(i < inBlock.size() -1){
		if(inBlock[i][0] == "SPACING"){
			this->name = "\0";
		}
		else if(inBlock[i][0] == "SAMENET"){
			this->SAMENETlayer1.push_back(inBlock[i][1]);
			this->SAMENETlayer2.push_back(inBlock[i][2]);
			this->SAMENETdim.push_back(stof(inBlock[i][3]));
			this->no_SAMENET++;
		}
		else{
			cout << "error in spacing" << endl;
		}
		i++;
	}

}

void lef_spacing::to_str(){
	cout << "Spacing Class:" << endl;
	cout << "\tID no: " << this->classID << endl;
	cout << "\tNo. samenet:" << this->no_SAMENET << endl;

	for(int i = 0; i < no_SAMENET; i++){
		cout << "\t" << i << "." << setw(12) << this->SAMENETlayer1[i] << setw(9) << this->SAMENETlayer2[i] << setw(4) << this->SAMENETdim[i] << endl;
	}

}

/** 
 *  Site class functions
 */


int lef_site::no_classes = 0;

lef_site::lef_site(vector<vector<string> > &inBlock){
	int i = 0;
	lef_site::no_classes++;
	no_total_classes++;
	this->classID = no_total_classes;

	while(i < inBlock.size() -1){
		if(inBlock[i][0] == "SITE"){
			this->name = inBlock[i][1];
		}
		else if(inBlock[i][0] == "SIZE"){
			this->SIZE[0] = stof(inBlock[i][1]);
			this->SIZE[1] = stof(inBlock[i][3]);
		}
		else if(inBlock[i][0] == "CLASS"){
			this->CLASS = inBlock[i][1];
		}
		else if(inBlock[i][0] == "SYMMETRY"){
			this->SYMMETRY = inBlock[i][1];
		}
		else{
			cout << "error in site" << endl;
		}
		i++;
	}

}

void lef_site::to_str(){
	cout << "Site Class:" << endl;
	cout << "\tID no: " << this->classID << endl;
	cout << "\tName: " << this->name << endl;
	cout << "\tSize: " << this->SIZE[0] << "x" << this->SIZE[1] << endl;
	cout << "\tClass: " << this->CLASS << endl;
	cout << "\tSymmetry: " << this->SYMMETRY << "-axis" << endl;
}

/** 
 *  Macro class functions
 */


int lef_macro::no_classes = 0;

lef_macro::lef_macro(vector<vector<string> > &inBlock){
	int i = 0;
	lef_macro::no_classes++;
	no_total_classes++;
	this->classID = no_total_classes;

	while(i < inBlock.size() -1){
		if(inBlock[i][0] == "MACRO"){
			this->name = inBlock[i][1];
		}
		else if(inBlock[i][0] == "CLASS"){
			this->CLASS = inBlock[i][1];
		}
		else if(inBlock[i][0] == "SIZE"){
			this->SIZE[0] = stof(inBlock[i][1]);
			this->SIZE[1] = stof(inBlock[i][3]);
		}
		else if(inBlock[i][0] == "ORIGIN"){
			this->ORIGIN[0] = stof(inBlock[i][1]);
			this->ORIGIN[1] = stof(inBlock[i][2]);
		}
		else if(inBlock[i][0] == "SYMMETRY"){
			this->SYMMETRY = inBlock[i][1];
		}
		else if(inBlock[i][0] == "SITE"){
			this->SITE = inBlock[i][1];
			this->SITEpar[0] = stof(inBlock[i][2]);
			this->SITEpar[1] = stof(inBlock[i][3]);
			this->SITEpar[2] = stof(inBlock[i][6]);
			this->SITEpar[3] = stof(inBlock[i][8]); 
			this->SITEpar[4] = stof(inBlock[i][10]);
			this->SITEpar[5] = stof(inBlock[i][11]);
		}
		else if(inBlock[i][0] == "PIN"){
			// creating a sub block
			vector<vector<string> > subBlock;
			subBlock.push_back(inBlock[i]);

			string blockName = inBlock[i][1];
			while(i++){
				subBlock.push_back(inBlock[i]);
				if(inBlock[i][0] == "END" && inBlock[i][1] == blockName)
					break;
			}

			int j = 0;
			while(j < subBlock.size()){
				macro_pin st_pin_Temp;
				if(subBlock[j][0] == "PIN"){
					this->PIN.resize(this->PIN.size()+1);
					this->PIN.back().name = subBlock[j][1];
				}
				else if(subBlock[j][0] == "DIRECTION"){
					this->PIN.back().DIRECTION = subBlock[j][1];
				}
				else if(subBlock[j][0] == "USE" && subBlock[j][1] == "SIGNAL"){
					if(subBlock[++j][0] == "PORT"){
						while(subBlock[++j][0] != "END"){
							if(subBlock[j][0] == "LAYER"){
								macro_port st_port_Temp;
								st_port_Temp.name = subBlock[j++][1];
								st_port_Temp.type = subBlock[j][0];
								st_port_Temp.dim[0] = stof(subBlock[j][1]);
								st_port_Temp.dim[1] = stof(subBlock[j][2]);
								st_port_Temp.dim[2] = stof(subBlock[j][3]);
								st_port_Temp.dim[3] = stof(subBlock[j][4]);

								this->PIN.back().PORT.push_back(st_port_Temp);
							}
							else
								cout << "macro pin error" << endl;
						}
					}
					else
						cout << "macro pin error" << endl;
				}
				else if(subBlock[j][0] == "END" && subBlock[j][1] ==  this->PIN.back().name){	
					break;
				}
				j++;
			}

		}
		else if(inBlock[i][0] == "OBS"){
			while(inBlock[i][0] != "END"){
				if(inBlock[i][0] == "LAYER"){
					macro_port st_port_Temp;
					st_port_Temp.name = inBlock[i++][1];
					st_port_Temp.type = inBlock[i][0];
					st_port_Temp.dim[0] = stof(inBlock[i][1]);
					st_port_Temp.dim[1] = stof(inBlock[i][2]);
					st_port_Temp.dim[2] = stof(inBlock[i][3]);
					st_port_Temp.dim[3] = stof(inBlock[i][4]);

					this->OBS.push_back(st_port_Temp);
				}
				i++;
			}
		}
		else{
			cout << "error in macro" << endl;
		}
		i++;
	}

}

void lef_macro::to_str(){
	cout << "Macro Class:" << endl;
	cout << "\tID no: " << this->classID << endl;
	cout << "\tName: " << this->name << endl;
	cout << "\tClass: " << this->CLASS << endl;
	cout << "\tSize: " << this->SIZE[0] << "x" << this->SIZE[1] << endl;
	cout << "\tOrigin: " << this->ORIGIN[0] << setw(4) << this->ORIGIN[1] << endl;
	cout << "\tSymmetry: " << this->SYMMETRY << "-axis" << endl;
	cout << "\tSite name: " << this->SITE << endl;
	cout << "\tSite par: "; printLineArrayF(this->SITEpar, 6);

	// pin.....
	cout << "\tPin Stuffs:" << endl;
	for(int i = 0; i < this->PIN.size(); i++){
		cout << "\t\tName: " << this->PIN[i].name << endl;
		cout << "\t\tDirection: " << this->PIN[i].DIRECTION << endl;

		cout << "\t\tPorts layers:" << endl;
		for(int j = 0; j < this->PIN[i].PORT.size(); j++){
			cout << "\t\t\tName: " << this->PIN[i].PORT[j].name << endl;
			cout << "\t\t\tType: " << this->PIN[i].PORT[j].type << endl;
			cout << "\t\t\tDim: "; printLineArrayF(this->PIN[i].PORT[j].dim, 4);
			}
		}
		cout << "\tOBS: " << endl;
		if(this->OBS.size() > 0){
			for(int i = 0; i < this->OBS.size(); i++){
				cout << "\t\tName: " << this->OBS[i].name << endl;
				cout << "\t\tType: " << this->OBS[i].type << endl;
				cout << "\t\tDim: "; printLineArrayF(this->OBS[i].dim, 4);
			}
		}
		else
			cout << "\t\tNULL" << endl;


}