/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-03-30
 * Modified:
 * license:
 * Description: Class for storing .def
 * File:				ParserDef.cpp
 */

#include "die2sim/ParserDef.hpp"

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

int def_file::importFile(const string &fileName){
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
				cout << "Skipping special nets..." << endl;
				// this->snets.resize(stoi(lineVec[1]));

				// unsigned int snetIndex = 0;
				// lineVec = splitFileLine(defFile);

				// while(lineVec[0] != "END" && lineVec[1] != "SPECIALNETS"){
        while(lineVec[0] != "END"){
          lineVec = splitFileLine(defFile);

					// while(lineVec[lineVec.size()-1] != ";"){
					// 	strBlock.push_back(lineVec);
					// 	lineVec = splitFileLine(defFile);
					// }
					// strBlock.push_back(lineVec);

					// // disVectorBlk(strBlock);
					// this->snets[snetIndex++].createAutoSpecial(strBlock);

					// strBlock.clear();
					// lineVec = splitFileLine(defFile);
				}
        cout << "Special nets, done." << endl;
			}
			else{
				cout << "Check for smoke." << endl;
			}
		}
		else if(validOHWords.find(keyword) != validOHWords.end()){
			// if(!proOH){
			// 	cout << "Processing overheads..." << endl;
			// 	proOH = true;
			// }
			// this->createAuto(lineVec);
		}
		else if(lineVec[0] == "END" && lineVec[1] == "DESIGN"){
			cout << "Importing DEF file done." << endl;
			break;
		}
		else{
			cout << "Unknown word." << endl;
      disVector(lineVec);
			return 0;
		}
	}

	defFile.close();

	return 1;
}

/**
 * [def_file::importNodesNets - Imports/converts the Nodes/Nets classes to DEF file format]
 * @param  inNodes [The input Nodes]
 * @param  inNets  [The input Nets]
 * @return         [1 - All good, 0 - Error]
 */

// int def_file::importNodesNets(vector<BlifNode> inNodes, vector<BlifNet> inNets){

//   cout << "Converting nodes and net into DEF format" << endl;

// 	this->comps.clear();
// 	this->nets.clear();
// 	this->snets.clear();

//   vector<unsigned int> nodesOutputCnt, nodesInputCnt;
//   nodesOutputCnt.resize(inNodes.size());
//   nodesInputCnt.resize(inNodes.size());

//   cout << "Processing nodes." << endl;

// 	for(auto &itNodes: inNodes){
// 		def_component tempComp;
// 		tempComp.name = itNodes.name;
// 		tempComp.compName = itNodes.GateType;
// 		tempComp.corX = itNodes.corX;
// 		tempComp.corY = itNodes.corY;
// 		this->comps.push_back(tempComp);
// 	}

//   cout << "Processing nets." << endl;

//   for(unsigned int i = 0; i < inNets.size(); i++){
//     if(inNets[i].outNodes.size() == 0 || inNets[i].inNodes.size() == 0){
//       continue;
//     }
// 	// for(auto &itNets: inNets){
// 		def_net tempNet;
//     unsigned int toComp, fromComp;
// 		string toCompType, fromComType;

//     tempNet.name = inNets[i].name;

//     fromComp = inNets[i].inNodes[0];
//     fromComType = inNodes[fromComp].GateType;
//     tempNet.fromComp = inNodes[fromComp].name;

//     toComp = inNets[i].outNodes[0];
//     toCompType = inNodes[toComp].GateType;
//     tempNet.ToComp = inNodes[toComp].name;

// 		// from/ output
// 		if(!fromComType.compare("PAD")){
// 			tempNet.fromPin = "INOUT_1";
// 		}
// 		else{
//       nodesOutputCnt[fromComp]++;
// 			tempNet.fromPin = "OUT_" + to_string(nodesOutputCnt[fromComp]);
// 		}

// 		// to/ input
// 		if(!toCompType.compare("PAD")){
// 			tempNet.ToPin = "INOUT_1";
// 		}
// 		else if(inNodes[toComp].clkNet == i){
//       tempNet.ToPin = "CLK";
// 		}
// 		else{
//       nodesInputCnt[toComp]++;
// 			tempNet.ToPin = "IN_" + to_string(nodesInputCnt[toComp]);
// 		}

//     this->nets.push_back(tempNet);

// 	}

//   cout << "Converting nodes and net into DEF format, done." << endl;
// 	return 1;
// }

// vector<string> splitFileLine(ifstream &inFile){
// 	string lineIn;
// 	char frontChar;

// 	static vector<string> errorVec = {"\0"};

// 	while(getline(inFile, lineIn)){
// 		frontChar = (char)lineIn.front();
// 		if(frontChar == '\n' || frontChar == '#' || frontChar == '\0'){			// skips commented and empty lines
// 			//just loop
// 		}
// 		else{
// 			return SplitStrVec(lineIn);
// 		}
// 	}
// 	cout << "Extracting data file file error!!!" << endl;
// 	return errorVec;
// }

/**
 * [def_file::PTLstats -  ]
 * @return [0 - Exit Success; 1 - Exit Failure]
 */

int def_file::PTLstats(){

  unsigned int lenMax = 0;       // Maximum length of the PTLs
  unsigned int lenMin = 10000;   // Minimum length of the PTLs
  float lenMean = 0;             // The mean/average length of the PTLs
  unsigned int lenCnt = 0;       // Number of PTLs
  unsigned long lenSum = 0;        // The sum of the PTL length
  float stdVar = 0;              // Standards variance


  cout << "Calculating stats on the transmission lines." << endl;
  lenCnt = this->nets.size();
  // for(unsigned int i = 0; i < lenCnt; i++){
  for(auto itnet: this->nets){
    lenSum += itnet.get_track_length();

    if(itnet.get_track_length() > lenMax)
     lenMax = itnet.get_track_length();

    if(itnet.get_track_length() < lenMin)
     lenMin = itnet.get_track_length();
  }

  lenMean = (float)lenSum / (float)lenCnt;

  // for(unsigned int i = 0; i < lenCnt; i++){
  for(auto itnet: this->nets){
    stdVar += pow((float)itnet.get_track_length() - lenMean, 2);
  }

  stdVar /= lenCnt;

  stdVar = sqrt(stdVar);

  cout << "PTL Stats:" << endl;
  cout << "\tCnt: "  << lenCnt << endl;
  cout << "\tMin: "  << lenMin << endl;
  cout << "\tMean: "  << lenMean << endl;
  cout << "\tMax: "  << lenMax << endl;
  cout << "\tTotal: "  << lenSum << endl;
  cout << "\tstdVar: "  << stdVar << endl;

  return 0;
}

/**
 * [def_file::to_def description]
 * @param  fileName [The file name of the to be created def file]
 * @return          [1 - Exit Success; 0 - Exit Failure]
 */

int def_file::to_def(const string &fileName){
	cout << "Creating def file: \"" << fileName << "\"" << endl;
  ofstream defFile;

  defFile.open(fileName);

  if(!defFile){
    cout << "Could not create \"" << fileName << "\"" << endl;
    return 0;
  }

  // ----------------------------- Header -----------------------------

  time_t now = time(0);
  char* currentTime = ctime(&now);

  defFile << "# Def file generated with ViPeR " << endl;
  defFile << "# Jude de Villiers, Stellenbosch University" << endl;
  defFile << "# " << currentTime << endl;

  // ----------------------------- Overheads -----------------------------

  defFile << "UNITS DISTANCE MICRONS 100 ;" << endl;
  // lefFile << "ROW core 1 505  N  DO 750 BY 1 STEP 1 1 ;" << endl;
  // lefFile << "TRACKS Y 0 DO 126 STEP 1000 LAYER metal1  ;" << endl;
  // lefFile << "GCELLGRID Y 1 DO 181 STEP 10 ;" << endl << endl;
  defFile << endl;

  // ----------------------------- Components -----------------------------

  defFile << "COMPONENTS " << this->comps.size() << " ;" << endl;

  for(auto &foo: this->comps){
    defFile <<  foo.to_def() << endl;
  }

  defFile << "END COMPONENTS" << endl << endl;

  // ----------------------------- Routes/Nets -----------------------------
  defFile << "NETS " << this->nets.size() + " ;" << endl;
  for(auto &foo: this->nets){
    defFile << foo.to_def() << endl;
  }


  defFile << "END NETS" << endl;

  defFile << "END DESIGN" << endl;
  defFile.close();
  cout << "Creating def file: \"" << fileName << "\" done." << endl;

	return 1;
}

/**
 * [def_file::genDot - Creates the tree diagram of the LEF file
 * dependencies]
 * @param  fileName [The file name of the to be created dot file]
 * @return          [0 - Exit Success; 1 - Exit Failure]
 */

int def_file::to_jpg(const std::string &fileName){
  cout << "Generating Dot file:\"" << fileName << "\" file" << endl;

  vector<string> fromSTR;
  vector<string> toSTR;

  const vector<string> clkKeys = {"SplitCLK", "SC", "clk", "CLK", "Clk"};
  // const string clkStr = "SplitCLK";
  // const string clkStr = "SC";
  const string inStr = "IN";
  const string outStr = "OUT";

  for(auto &fooNet: this->nets){
    if(!fuzzySearch(fooNet.get_varFromComp(), clkKeys) 
      && !fuzzySearch(fooNet.get_varToComp(), clkKeys)){
  	// if((fooNet.get_varFromComp().find(clkStr) == string::npos)
  	// 		&& fooNet.get_varToComp().find(clkStr) == string::npos){
  		if(fooNet.get_varFromPin().find(inStr) == string::npos){
  			if(fooNet.get_varToPin().find(outStr) == string::npos){
	  			fromSTR.push_back(fooNet.get_varFromComp());
				  toSTR.push_back(fooNet.get_varToComp());
				}
				else{
	  			toSTR.push_back(fooNet.get_varFromComp());
				  fromSTR.push_back(fooNet.get_varToComp());
				}
  		}
  		else{
  			toSTR.push_back(fooNet.get_varFromComp());
			  fromSTR.push_back(fooNet.get_varToComp());
  		}
  	}
  }


  // ------------------------ creating dot file ------------------------

  FILE *dotFile;
  dotFile = fopen("foo.dot", "w");

  string lineStr;

  lineStr = "digraph DEFtree {\n";
  fputs(lineStr.c_str(), dotFile);

  for (unsigned int i = 0; i < fromSTR.size(); i++) {
    lineStr = "\t" + fromSTR[i] + " -> " + toSTR[i] + ";\n";
    fputs(lineStr.c_str(), dotFile);
  }

  lineStr = "}";
  fputs(lineStr.c_str(), dotFile);

  fclose(dotFile);
  cout << "Dot file done." << endl;

  // ------------------------ executing dot script ------------------------
  string bashCmd;
  bashCmd = "dot -Tjpg foo.dot -o " + fileName;

  if (system(bashCmd.c_str()) == -1) {
    cout << "Bash command :\"" << bashCmd << "\" error." << endl;
    return 1;
  }

  bashCmd = "rm foo.dot";
  if (system(bashCmd.c_str()) == -1) {
    cout << "Bash command :\"" << bashCmd << "\" error." << endl;
    return 1;
  }

  return 0;
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
	this->corX = (int)stod(inLine[6]);
	this->corY = (int)stod(inLine[7]);
	this->orient = inLine[9];

	return 1;
}

void def_component::to_str(){
	cout << "Component Class:" << endl;
	cout << "\tName: " << this->name << endl;
	cout << "\tComponent: " << this->compName << endl;
	cout << "\tPosition type: " << this->posType << endl;
	cout << "\tPosition(x,y): " << this->corX << "  " << this->corY << endl;
	cout << "\tOrientation: " << this->orient << endl;
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

double def_net::get_trans_delay(){
	double trackLen = 0;

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

	// cout << "L: " << trackLen << endl;
	// cout << "f: " << this->speedConstant << endl;

	return trackLen * this->speedConstant;
}

/**
 * [def_net::get_track_length - calculates the length of the track in nm]
 * @return [description]
 */

unsigned int def_net::get_track_length(){
  unsigned int trackLen = 0;

  for(unsigned int i = 0; i < this->routes.size(); i++){

    for(unsigned int j = 1; j < this->routes[i].ptX.size(); j++){
      if(routes[i].ptX.size() == 1){    // can be omitted
        // Skips single point/dimension tracks, can be due to via placement
        continue;
      }

      trackLen += abs(this->routes[i].ptX[j] - this->routes[i].ptX[j-1]) *10;
      trackLen += abs(this->routes[i].ptY[j] - this->routes[i].ptY[j-1]) *10;
    }
  }

  return trackLen;
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
/**
 * [fuzzySearch - searching a string with a set(vector) of keys]
 * @param  word [The string in which much be searched]
 * @param  keys [vector of key words to search for]
 * @return      [true if found else false]
 */
bool fuzzySearch(string word, vector<string> keys){

  for(const auto &key: keys){
    if(word.find(key) != string::npos){
      return true;
    }
  }

  return false;
}