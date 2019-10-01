/**
 * Author:  		Jude de Villiers
 * Origin:  		E&E Engineering - Stellenbosch University
 * For:					Supertools, Coldflux Project - IARPA
 * Created: 		2019-05-20
 * Modified:
 * license: 
 * Description: Reads in LEF/DEF/GDS files are displays the content.
 * File:				decipher.cpp
 */

#include "die2sim/ToText.hpp"

/**
 * [decipherFile - Reads in LEF/DEF/GDS files are displays the content]
 * @param  fileName [LEF/DEF/GDS file name]
 * @return          [Returns 1 if all good else 0 if error]
 */

int decipherFile(string fileName){

	cout << "Deciphering \"" << fileName << "\"" << endl;

  size_t foo;

  foo = fileName.rfind(".lef");
  if(foo != string::npos){
		lefimport(fileName);
		disLEFdata();
  	return 1;
  }

  foo = fileName.rfind(".def");
  if(foo != string::npos){
		defimport(fileName);
		disDEFdata();
  	return 1;
  }

  foo = fileName.rfind(".gds");
  if(foo != string::npos){
  	GDSfile2ASCII(fileName);
  	return 1;
  }

  foo = fileName.rfind(".gds2");
  if(foo != string::npos){
  	GDSfile2ASCII(fileName);
  	return 1;
  }


  cout << "File naming error." << endl;

	return 0;
}