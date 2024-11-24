#include <iostream>
#include "pieces.h"
#include "types.h"
using namespace std;

int main(){
	/*
	Pieces pieces = Pieces();
	cout << pieces.generateWhitePawnMoves() << endl;
	cout << pieces.generateWhiteKingMoves() << endl;
	cout << pieces.generateWhiteKnightMoves() << endl;
	*/

	for (int i = H4; i <= A8; i++){
		cout << "0x" << hex << bitset(i) << "ULL, "<< endl;
	}
	return 0;

}
