/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-04-25
 * Modified:
 * license:
 * Description: Passer to create a JoSIM file.
 * File:				ParserJosim.cpp
 */

#include "die2sim/ParserJosim.hpp"

int JoSimFile::no_JoSim_classes = 0;

/**
 * [JoSimFile::genCir - Generates the final .cir file in the correct formate]
 * @return [1 - all good; 0 - error]
 */

int JoSimFile::genCir(string fileName){
	cout << "Creating JoSIM file -> " << fileName << endl;

	// import the sub-circuits
	cout << "Importing:" << endl;
	for(int i = 0; i < this->fileNamesImport.size(); i++){
		cout << "  [" << i + 1 << "]: " << this->fileNamesImport[i] << endl;
		cpFile(this->fileNamesImport[i], fileName);
	}

	FILE *outFile;
  outFile = fopen(fileName.c_str(), "a");


  // Create a sub-circuit of LEF file
  fputs(makeHeader(this->subcktName).c_str(), outFile);
  fputs(this->createSubcktHeader().c_str(), outFile);

  // creating components
  fputs(makeHeader("Components").c_str(), outFile);
  for(unsigned int i = 0; i < this->placeComp.size(); i++){
	  fputs(this->placeComp[i].c_str(), outFile);
	  fputs("\n", outFile);
  }

  // Creates transmission lines
  fputs(makeHeader("Transmission Lines").c_str(), outFile);
  for(unsigned int i = 0; i < this->placeNet.size(); i++){
	  fputs(this->placeNet[i].c_str(), outFile);
	  fputs("\n", outFile);
  }

  // ends the sub-circuit of LEF file
  string foo = ".ends " + this->subcktName + "\n";
  fputs(foo.c_str(), outFile);

  fclose(outFile);

	return 1;
}

/**
 * [JoSimFile::importCir - Adds the name of the .cir files to be imported]
 * @param fileName [Name of the .cir file]
 */

int JoSimFile::importCir(string fileName){
	this->fileNamesImport.push_back(fileName);
	return 1;
};

/**
 * [JoSimFile::addComp - Adds components to the file]
 * @param  compName [Name of component]
 * @param  compType [Name of the type of component]
 * @param  nets     [List of the connections]
 * @return          [1 - all good; 0 - error]
 */

int JoSimFile::addComp(string compName, string compType, vector<string> &nets){
	string netsStr = "";

	for(int i = 0; i < nets.size(); i++){
		netsStr = netsStr + "\t\t" +nets[i];
	}

	if(compType == "PAD"){
		appendSubcktPort(nets[0], compName);
	}
	else {
		this->placeComp.push_back("X" + compName + "\t\t" + compType + netsStr);
	}

	return 1;
}

/**
 * [JoSimFile::addTrans - Adds transmission lines to the file]
 * @param  netName [Name of the net]
 * @param  nets    [Size 4 vector of the connected nets]
 * @param  impe    [Route Impedance]
 * @param  tDelay  [Time delay value, must be in picoseconds]
 * @return         [1 - all good; 0 - error]
 */

int JoSimFile::addTrans(string netName, vector<string> &nets, float impe, float tDelay){
	string netsStr = "\t\t";

	// setting precision
	stringstream tDelayST;
	stringstream impeST;
	tDelayST << fixed << setprecision(2) << tDelay;
	impeST << fixed << setprecision(2) << impe;


	string paraStr = "\t\tLOSSLESS Z0=" + impeST.str() + "\tTD=" + tDelayST.str() + "p";

	for(int i = 0; i < nets.size(); i++){
		netsStr = netsStr + "\t\t" +nets[i];
	}

	this->placeNet.push_back("T" + netName + netsStr + paraStr);

	return 1;
}

/**
 * [JoSimFile::appendSubcktPort description]
 * @param  netName [The name of the net connection]
 * @param  netDes  [Description of the net connection]
 * @return         [1 - all good; 0 - error]
 */

int JoSimFile::appendSubcktPort(string netName, string netDes){
	subcktNetName.push_back(netName);
	subcktNetDes.push_back(netDes);

	return 1;
}

/**
 * [JoSimFile::createSubcktHeader - Creates the header for the main sub-circuit]
 * @return [The built string for the header]
 */

string JoSimFile::createSubcktHeader(){
	string subcktStr = "* ";
	for(unsigned int i = 0; i < this->subcktNetDes.size(); i++){
	  subcktStr += "\t" + subcktNetDes[i];
	}

  subcktStr += "\n.SUBCKT " + this->subcktName;

  for(unsigned int i = 0; i < this->subcktNetName.size(); i++){
	  subcktStr += "\t" + subcktNetName[i];
	}

	return subcktStr;
}

/**
 * [JoSimFile::to_str - Prints the class information to the terminal]
 */

void JoSimFile::to_str(){
	// imported .cir files
	cout << "Name of the files to me imported:" << endl;
	for(int i = 0; i < this->fileNamesImport.size(); i++){
		cout << "[" << i << "]: " << this->fileNamesImport[i] << endl;
	}
	cout << endl;

	// components added
	cout << "Components added:" << endl;
	for(int i = 0; i < this->placeComp.size(); i++){
		cout << "[" << i << "]: " << this->placeComp[i] << endl;
	}
}

/**
 * [cpFile - Copies one text file to another]
 * @param  fromFile [Name of the file to be copied from]
 * @param  toFile   [Name of the file to be copied to]
 * @return          [1 - all good; 0 - error]
 */

int cpFile(string fromFile, string toFile){
  FILE *inFile, *outFile;
  char c;

  inFile = fopen(fromFile.c_str(), "r");
	if(inFile==NULL){
  	cout << "Could not open \"" << fromFile.c_str() << "\" for copying" << endl;
  	return 0;
  }

  outFile = fopen(toFile.c_str(), "w");
	if(inFile==NULL){
  	cout << "Could not create \"" << toFile.c_str() << "\" for copying" << endl;
  	return 0;
  }

  c = fgetc(inFile);
  while(c != EOF){
	  fputc(c, outFile);
	  c = fgetc(inFile);
  }

  fclose(inFile);
  fclose(outFile);

	return 1;
}

/**
 * [makeHeader - Creates a paragraph header]
 * @param  HeaderName [The name of the header]
 * @return            [The created header]
 */

string makeHeader(string HeaderName){
	string foo;

	foo = "\n\n* ==========================================================================\n* ";

	for(int i = 0; i < (74 - HeaderName.length())/2-1; i++){
		foo = foo + "=";
	}

	foo = foo + " " + HeaderName + " ";

	for(int i = 0; i < (74 - HeaderName.length())/2-1; i++){
		foo = foo + "=";
	}

	foo = foo + "\n* ==========================================================================\n\n";

	return foo;
}