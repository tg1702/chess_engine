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


	 cout << "Rook magic numbers: " << endl;
	 for (int i = 0; i < 64; i++){		
		cout << generateMagicNumber(i, generateWhiteRookMask(i)) << "," << endl;
	}	


	return 0;
}
