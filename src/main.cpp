#include <iostream>
#include "pieces.h"
#include "types.h"
using namespace std;

int main(){
	
	PieceManager pieces = PieceManager();


	for (int i = 0; i < 64; i++){
		PieceArgs args {.w_knights_bb = bitset(i)};
		pieces.setBoard(args);
		cout << "0x" << hex << pieces.getMovesBB(WHITE, KNIGHT) << "ULL, "<< endl;
	}
	return 0;

}
