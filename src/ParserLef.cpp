/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-04-25
 * Modified:
 * license: 
 * Description: Parser for .LEF
 * File:				ParserLef.cpp
 */

#include "die2sim/ParserLef.hpp"

set<string> validOthWords = {"NAMESCASESENSITIVE", "LIBRARY", "END"};
set<string> validClassWords = {"UNITS", "LAYER", "MACRO", "VIA", "OBS", "SPACING", "SITE"};
set<string> validNamelessWords = {"UNITS", "SPACING"};
ifstream lefFile;
// vector<vector<string> > strBlock;
vector<lef_layer> lef_cl_layer;
vector<lef_via> lef_cl_via;
vector<lef_site> lef_cl_site;
vector<lef_macro> lef_cl_macro;
lef_spacing lef_cl_spacing;
lef_units lef_cl_units;
map<string, int> LayerAssign;
map<string, int> LayerTrackWidth;

int lefimport(string LefFileName){
	cout << "Importing LEF file -> "  << LefFileName << endl; 
	vector<vector<string> > strBlock;
	string keyword;
	vector<string> lineVec;
	string tempDis;
	string blockName;

	bool proLAYER = false;
	bool proMACRO = false;
	bool proVIA = false;
	bool proSITE = false;
	bool proSPACEING = false;
	bool proUNITS = false;

	lefFile.open(LefFileName);

	if(!lefFile.is_open()){
		cout << "LEF file FAILED to be properly opened" << endl;
		return 0;
	}

  //while(getline(lefFile, lineIn)){
  while(proNewLineLef(lineVec)){

		keyword = lineVec[0];
  	if (validClassWords.find(keyword) != validClassWords.end()){
  		// keyword found, build block
  		
  		createBlockLef(lineVec, strBlock);

  		if(keyword == "LAYER"){
  			if(!proLAYER){
					cout << "Processing layers..." << endl; 
					proLAYER = true;
				}
  			lef_layer cl_layer_Temp(strBlock);
  			lef_cl_layer.push_back(cl_layer_Temp);
  			// lef_cl_layer.back().to_str();
  		}
  		else if(keyword == "MACRO"){
  			 if(!proMACRO){
					cout << "Processing macros..." << endl; 
					proMACRO = true;
				}
  			lef_macro cl_macro_Temp(strBlock);
  			lef_cl_macro.push_back(cl_macro_Temp);
  			// lef_cl_macro.back().to_str();
  		}
  		else if(keyword == "VIA"){
  			if(!proVIA){
					cout << "Processing vias..." << endl; 
					proVIA = true;
				}
  			lef_via cl_via_Temp(strBlock);
  			lef_cl_via.push_back(cl_via_Temp);
  			// lef_cl_via.back().to_str();
  		}
  		else if(keyword == "SITE"){
  			if(!proSITE){
					cout << "Processing sites..." << endl; 
					proSITE = true;
				}
  			lef_site cl_site_Temp(strBlock);
  			lef_cl_site.push_back(cl_site_Temp);
  			// lef_cl_site.back().to_str();
  		}
  		else if(keyword == "SPACING"){
  			if(!proSPACEING){
					cout << "Processing spacings..." << endl; 
					proSPACEING = true;
				}
  			lef_cl_spacing.createAuto(strBlock);
  			// lef_cl_spacing.to_str();
  		}
  		else if(keyword == "UNITS"){
  			if(!proUNITS){
					cout << "Processing units..." << endl; 
					proUNITS = true;
				}
  			lef_cl_units.createAuto(strBlock);
  			// lef_cl_units.to_str();
  		}
  		else{
  			cout << "Word is valid, no class created yet." << endl;
  		}
  	}
  	else if(validOthWords.find(keyword) != validOthWords.end()){
			// other keyword found
			if(keyword == "NAMESCASESENSITIVE"){
				// assign some stuffs
				// cout << "Stuffs is cases sensitive." << endl;
			}
			else if(keyword == "END"){
				if(lineVec[1] == "LIBRARY"){
					cout << "Importing LEF file done." << endl;
				}
				else cout << "Found an alone END, miss match..." << endl;
			}
			else {
				cout << "Word is valid, no function created yet." << endl;
			}

		}
		else {
			// word not in the library, out error;

  		cout << "Error with line ->" << endl;
  		disVector(lineVec);
  		cout << "<--------------------" << endl;

  		cout << "String not found in lef dictionary" << endl;
  		cout << "Totsiens!" << endl;
  		return 0;
		}
 

  }
	lefFile.close();
	return 1;
}

/**
 * Creates a block(vector x vector) of string of a section(until an end) in the lef life 
 * @param  inVec [the first line that was already read]
 * @param  vecBlock [The input string that needs to be separated by whitespaces]
 * @return   [returns 1 if successful and 0 if error occurred]
 */

int createBlockLef(vector<string> &inVec, vector<vector<string> > &strBlock){
	// vector<vector<string> > strBlock;
	strBlock.clear();
	strBlock.push_back(inVec);

	string blockName = inVec[1];

	//for(int i = 0; i < LEFmaxClassLines; i++){					// can use while loop, but for loop is used for safety
	while(proNewLineLef(inVec)){													// figure something better
		strBlock.push_back(inVec);
		if(inVec[0] == "END" && (inVec[1] == blockName || validNamelessWords.find(inVec[1]) != validNamelessWords.end())) break;
		//if(i == LEFmaxClassLines -1) cout << "block line limit has been reached" << endl;
		
	}

	return 1;
}

/**
 * Process new line of lef file
 * Fetches the new line from the lef file, removes the whitespaces and separates the data into a vector
 * @return       [return the output of the getline function]
 * @param  inVec [The output VECTOR where separated strings are stored]
 */

int proNewLineLef(vector<string> &inVec){
	string lineIn;
	char frontChar;

	while(getline(lefFile, lineIn)){
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


/**
 * [disLEFdata - Displays all the .to_str of all the classes]
 */

void disLEFdata(){

	// Layers
	// for(unsigned int i = 0; i < lef_cl_layer.size(); i++) lef_cl_layer[i].to_str();

	// vias
	// for(unsigned int i = 0; i < lef_cl_via.size(); i++) lef_cl_via[i].to_str();

	// Site
	// for(unsigned int i = 0; i < lef_cl_site.size(); i++) lef_cl_site[i].to_str();

	// Macro
	for(unsigned int i = 0; i < lef_cl_macro.size(); i++) lef_cl_macro[i].to_str();

	// Spacing
	// lef_cl_spacing.to_str();
	
	// Units
	// lef_cl_units.to_str();
}

int getLayerNo(string LayerName){
	static bool firstRun = false;
	if(!firstRun){
		for(int i = 0; i < lef_cl_layer.size(); i++){
			LayerAssign.insert(pair<string, int>(lef_cl_layer[i].get_varName(), i));
		}
		firstRun = true;
	}

	map<string,int>::iterator foo;
	foo = LayerAssign.find(LayerName);

	if(foo == LayerAssign.end()){
		cout << "Layer not found->" << LayerName << "<-" << endl;
		return -1;
	}

  return foo->second;
}

// map<string, int> LayerTrackWidth;
int getTrackWidth(string LayerName){
	static bool firstRun = false;
	if(!firstRun){
		for(int i = 0; i < lef_cl_layer.size(); i++){
			LayerTrackWidth.insert(pair<string, int>(lef_cl_layer[i].get_varName(), lef_cl_layer[i].get_varWidth()));
		}
		firstRun = true;
	}

	map<string,int>::iterator foo;
	foo = LayerTrackWidth.find(LayerName);

	if(foo == LayerTrackWidth.end()){
		cout << "Layer not found->" << LayerName << "<-" << endl;
		return -1;
	}

  return foo->second;
}

void getViaVec(vector<lef_via> &outVec){
	outVec = lef_cl_via;
}

void getMacroVec(vector<lef_macro> &outVec){
	outVec = lef_cl_macro;
}