#include <iostream>
#include "movegen.h"


int main(){
PieceManager pieces = PieceManager();
MoveList moves = MoveList();

PieceBB white {.king_bb = W_KING_START, .queen_bb = W_QUEEN_START, .rook_bb = W_ROOKS_START, .bishop_bb = W_BISHOPS_START, .knight_bb = W_KNIGHTS_START, .pawn_bb = W_PAWNS_START};
uint64_t all_black = B_ALL_PIECES_START;
moves.generateMoves(WHITE, &white, all_black);
std::vector<Move> m = moves.getMoves();
std::cout << "size " << m.size() << '\n';
for (const auto& mv: m){
	mv.printMove();
}
return 0;
}
