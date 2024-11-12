#include <iostream>
#include "pieces.h"
using namespace std;

int main(){
	Pieces pieces = Pieces();
	cout << pieces.generateWhitePawnMoves() << endl;
	cout << pieces.generateWhiteKingMoves() << endl;
	cout << pieces.generateWhiteKnightMoves() << endl;	
	return 0;

}
