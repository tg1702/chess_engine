#include <iostream>
#include <vector>
#include "move.h"
#include "pieces.h"
#include "types.h"
#include "utils.h"
#include "movegen.h"

void MoveList::generateMoves(bool side, const PieceBB* friendly, uint64_t all_enemy){
	uint64_t all_friendly = friendly->king_bb | friendly->knight_bb | friendly->pawn_bb | friendly->rook_bb | friendly->bishop_bb | friendly->queen_bb;	
	clearMoves();
	generateKingMoves(side, friendly->king_bb, all_friendly);
	generateKnightMoves(side, friendly->knight_bb, all_friendly);
	generatePawnMoves(side, friendly->pawn_bb, all_friendly, all_enemy);
	generateRookMoves(side, friendly->rook_bb, all_friendly, all_enemy);
	generateBishopMoves(side, friendly->bishop_bb, all_friendly, all_enemy);
	generateQueenMoves(side, friendly->queen_bb, all_friendly, all_enemy);
}

bool MoveList::isPromoting(bool side, int from, int to){

                if (side == WHITE && (bitset(from) & RANK_7) && (bitset(to) & RANK_8)){
                        return true;
                }
                else if (side == BLACK && (bitset(from) & RANK_2) && (bitset(to) & RANK_1)){
                        return true;
                }
                else{
                        return false;
                }

}

void MoveList::addPossibleMove(int start, uint64_t positions, bool side, piece_bb_types type){
	while (positions != 0ULL){
                int to = __builtin_ctzll(positions);

                if (type == PAWN && isPromoting(side, start, to)){
                        for(auto& promoted_code: promoted_piece_codes){
                                moves[move_count] = Move(promoted_code, start, to, type);

                        }
                }
		else{
                        
			moves[move_count] = (Move(NORMAL, start, to, type));
                }

		move_count += 1;
                positions = bitclear(positions, to);

        }
           

}
void MoveList::generateKingMoves(bool side, uint64_t kingbb, uint64_t blockers){
        uint64_t bitboard = kingbb;
        uint64_t kingMoves = 0ULL;

        while (bitboard != 0ULL){
                int index = __builtin_ctzll(bitboard);

                kingMoves |= kingLookups[index] & ~(blockers);
                addPossibleMove(index, kingLookups[index] & ~(blockers), side, KING);
       		 
                bitboard = bitclear(bitboard, index);
	}


}

void MoveList::generateKnightMoves(bool side, uint64_t knightbb, uint64_t sameSidePieces){
 	uint64_t bitboard = knightbb;
 	uint64_t knightMoves = 0ULL;

 while (bitboard != 0ULL){
        int index = __builtin_ctzll(bitboard);

        knightMoves |= knightLookups[index] & ~(sameSidePieces);


        addPossibleMove(index, knightLookups[index] & ~(sameSidePieces), side, KNIGHT);

        bitboard = bitclear(bitboard, index);
 }

}

void MoveList::generatePawnMoves(bool side, uint64_t pawnbb, uint64_t myBlockers, uint64_t oppBlockers){
        uint64_t bitboard = pawnbb;
        uint64_t pawnMoves = 0ULL;
        uint64_t all = myBlockers | oppBlockers;
       
	
	while (bitboard != 0ULL){
                int index = __builtin_ctzll(bitboard);

                uint64_t double_pawn_pushes = 0ULL;

                if (side == WHITE && (bitboard & bitset(index) & RANK_2)&& (pawnLookups[side][index] & ~all) && (bitset(index+16) & ~all)) {double_pawn_pushes = bitset(index+16);}
                else if (side == BLACK && (bitboard & bitset(index) & RANK_7) && (pawnLookups[side][index] & ~all)  && (bitset(index-16) & ~all)) {double_pawn_pushes = bitset(index-16);}

                pawnMoves |= (((pawnLookups[side][index])) & (~all)) | (double_pawn_pushes) |  (pawnAttackLookups[side][index] & oppBlockers );
                addPossibleMove(index, ((pawnLookups[side][index] & (~all))  | (double_pawn_pushes) | (pawnAttackLookups[side][index] & oppBlockers)), side, PAWN);


                bitboard = bitclear(bitboard, index);
        }

}

void MoveList::generateRookMoves(bool side, uint64_t rookbb, uint64_t myBlockers, uint64_t oppBlockers){
        uint64_t bitboard = rookbb;
        uint64_t rookMoves = 0ULL;

	uint64_t all = oppBlockers | myBlockers;

        while (bitboard != 0ULL){
                int square = __builtin_ctzll(bitboard);


                int index = utils::generateMagicIndex( all & rookOccupancyMasks[square], rookMagics[square], square, 0);

                rookMoves |= rookMoveList[square][index] & ~(myBlockers);

                addPossibleMove(square, rookMoveList[square][index] & ~(myBlockers), side, ROOK);

                bitboard = bitclear(bitboard, square);
        }


}
void MoveList::generateBishopMoves(bool side, uint64_t bishopbb, uint64_t myBlockers, uint64_t oppBlockers){
        uint64_t bitboard = bishopbb;
        uint64_t bishopMoves = 0ULL;
	uint64_t all = myBlockers | oppBlockers;

        while (bitboard != 0ULL){

                int square = utils::countr_zero(bitboard);


                int index = utils::generateMagicIndex((all) & bishopOccupancyMasks[square], bishopMagics[square], square, 1);

                bishopMoves |= bishopMoveList[square][index] & ~(myBlockers);

                addPossibleMove(square, bishopMoveList[square][index] & ~(myBlockers), side, BISHOP);



                bitboard = bitclear(bitboard, square);
        }


}

void MoveList::generateQueenMoves(bool side, uint64_t queenbb, uint64_t myBlockers, uint64_t oppBlockers){
  	uint64_t bitboard = queenbb;
        uint64_t rookMoves = 0ULL;
        uint64_t bishopMoves = 0ULL;
	uint64_t all = myBlockers | oppBlockers;

        while (bitboard != 0ULL){
                int square = utils::countr_zero(bitboard);


                int rookIndex = utils::generateMagicIndex((all) & rookOccupancyMasks[square], rookMagics[square], square, 0);

                int bishopIndex = utils::generateMagicIndex((all) & bishopOccupancyMasks[square], bishopMagics[square], square, 1);


                rookMoves |= rookMoveList[square][rookIndex] & ~(myBlockers);
                bishopMoves |= bishopMoveList[square][bishopIndex] & ~(myBlockers);


                addPossibleMove(square, (bishopMoveList[square][bishopIndex] & ~(myBlockers) | (rookMoveList[square][rookIndex] & ~(myBlockers))), side, QUEEN);

                bitboard = bitclear(bitboard, square);
        }

}

void MoveList::addKingSideCastling(bool side){
        Move castlingMove;
	(side == WHITE) ? castlingMove = Move(W_KS_CASTLE_FLAG, E1, G1, KING) : castlingMove = Move(B_KS_CASTLE_FLAG, E8, G8, KING);
	moves[move_count] = castlingMove;
	move_count += 1;
}

void MoveList::addQueenSideCastling(bool side){
        Move castlingMove;
        (side == WHITE) ? castlingMove = Move(W_QS_CASTLE_FLAG, E1, C1, KING) : castlingMove = Move(B_QS_CASTLE_FLAG, E8, C8, KING);
	moves[move_count] = castlingMove;
	move_count += 1;
}
