#include <iostream>
#include "pieces.h"
#include "types.h"
#include "lookup.h"
using namespace std;

int main(){
	
	PieceManager pieces = PieceManager();

	/*

	for (int i = 0; i < 64; i++){
		PieceArgs args {.w_knights_bb = bitset(i)};
		pieces.setBoard(args);
		cout << "0x" << hex << pieces.getMovesBB(WHITE, KNIGHT) << "ULL, "<< endl;
	}

	*/

	uint64_t bitboard = 0ULL;
	uint64_t captureMoves = 0ULL;

	cout << "{ ";
	for (int i = 0; i < 64; i++){
		bitboard = bitset(i);
		captureMoves = (bitboard << 7 | bitboard << 9) & outputRank(bitboard << 8);
		cout << captureMoves << ", ";
	}
	cout << "}";

	cout << endl;

	cout << "{ ";
	for (int i = 63; i >= 0; i--){

		bitboard = bitset(i);
		captureMoves = (bitboard >> 7 | bitboard >> 9) & outputRank(bitboard >> 8);
		cout << captureMoves << ", ";
	}

	cout << "}" << endl;
	return 0;

}
