#include <iostream>
#include "lookup.h"
#include "types.h"
#include "pieces.h"
#include "magic.h"
using namespace std;


int main(){


	//	MAGIC NUMBER GENERATION
	//
	//	Run the following to obtain magic numbers
	//
	//	Corresponding lookup table values are in "magic.txt"
	//
	//	Add the values in magic.txt to "magic.h"

	// ROOK MAGICS

 for (int i = 0; i <64; ++i){
    cout << generateMagicNumber(i, generateWhiteRookMask(i), 1) << "ULL, " << endl;
 }

	// BISHOP MAGICS	
for (int i = 0; i < 64; i++){		 
		cout << generateMagicNumber(i, generateWhiteBishopMask(i), 1) << "ULL, " << endl;
	}	


	  
	 
	

	return 0;

}
