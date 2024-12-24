#include <iostream>
#include <vector>
#include "move.h"
#include "pieces.h"
#include "types.h"
#include "utils.h"
#include "movegen.h"

void MoveGen::generateMoves(bool side, const PieceBB* friendly, uint64_t all_enemy, MoveList* move_list, int enPassantSquare){
	uint64_t all_friendly = friendly->king_bb | friendly->knight_bb | friendly->pawn_bb | friendly->rook_bb | friendly->bishop_bb | friendly->queen_bb;	
	generateKingMoves(side, friendly->king_bb, all_friendly, move_list);
	generateKnightMoves(side, friendly->knight_bb, all_friendly, move_list);
	
	(side == WHITE) ? generateWhitePawnMoves(side, friendly->pawn_bb, all_friendly, all_enemy, move_list, enPassantSquare) : generateBlackPawnMoves(side, friendly->pawn_bb, all_friendly, all_enemy, move_list, enPassantSquare);

	generateRookMoves(side, friendly->rook_bb, all_friendly, all_enemy, move_list);
	generateBishopMoves(side, friendly->bishop_bb, all_friendly, all_enemy, move_list);
	generateQueenMoves(side, friendly->queen_bb, all_friendly, all_enemy, move_list);

}

bool MoveGen::isPromoting(bool side, int from, int to){

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

void MoveGen::addPossibleMove(int start, uint64_t positions, bool side, int type, MoveList* move_list){

	
	while (positions != 0){
                int to = utils::pop_lsb(positions);
		move_list->moves[move_list->count] = Move(NORMAL, start, to, type);			// }
		move_list->count++;

        }
           

}
void MoveGen::generateKingMoves(bool side, uint64_t kingbb, uint64_t blockers, MoveList* move_list){

        while (kingbb != 0ULL){
                int from = utils::pop_lsb(kingbb);
		
		uint64_t legalMove = kingLookups[from] & ~(blockers);
                
		addPossibleMove(from, legalMove, side, KING, move_list);
       		 
	}


}

void MoveGen::generateKnightMoves(bool side, uint64_t knightbb, uint64_t sameSidePieces, MoveList* move_list){

 while (knightbb != 0ULL){
        int from = utils::pop_lsb(knightbb);

	uint64_t legalMove = knightLookups[from] & ~(sameSidePieces);


        addPossibleMove(from, legalMove, side, KNIGHT, move_list);

 }
}

void MoveGen::generateWhitePawnMoves(bool side, uint64_t pawnbb, uint64_t myBlockers, uint64_t oppBlockers, MoveList* move_list, int enPassantSquare){
	uint64_t all = myBlockers | oppBlockers;
       	uint64_t empty = ~all;
	uint64_t ep_squares = (enPassantSquare == -1) ? 0 : (pawnbb & RANK_5 & ( bitset((enPassantSquare - 1)) | bitset((enPassantSquare + 1))));

	
	while (pawnbb != 0ULL){
                int from = utils::pop_lsb(pawnbb);

		uint64_t single_pawn_pushes = pawnLookups[side][from] & empty;
                
		uint64_t shift = single_pawn_pushes<<8;
		uint64_t double_pawn_pushes = shift & empty & RANK_4;


                uint64_t legalMove = single_pawn_pushes | double_pawn_pushes  | (pawnAttackLookups[side][from] & oppBlockers );
                
		while ( legalMove != 0ULL){
			int to = utils::pop_lsb(legalMove);
			move_list->moves[move_list->count] = Move(NORMAL, from, to, PAWN);
			move_list->count++;

		}

        }



	while (ep_squares != 0ULL){	
		int from = utils::pop_lsb(ep_squares);
		move_list->moves[move_list->count] = Move(EN_PASSANT_FLAG, from, enPassantSquare + 8, PAWN, PAWN);
		move_list->count++;		
		
	}
	
	//std::cout << "---------------------------------" << '\n';	
}

void MoveGen::generateBlackPawnMoves(bool side, uint64_t pawnbb, uint64_t myBlockers, uint64_t oppBlockers, MoveList* move_list, int enPassantSquare){
 	uint64_t all = myBlockers | oppBlockers;
        uint64_t empty = ~all;
	uint64_t ep_squares = (enPassantSquare == -1) ? 0: pawnbb & RANK_4 & ( bitset((enPassantSquare - 1)) | bitset((enPassantSquare + 1))) ;
        
	while (pawnbb != 0ULL){
                int from = utils::pop_lsb(pawnbb);

                uint64_t single_pawn_pushes = pawnLookups[side][from] & empty;

                uint64_t shift = single_pawn_pushes>>8;
                uint64_t double_pawn_pushes = shift & empty & RANK_5;


                uint64_t legalMove = single_pawn_pushes | double_pawn_pushes  | (pawnAttackLookups[side][from] & oppBlockers );

		while ( legalMove != 0ULL){
			int to = utils::pop_lsb(legalMove);
			move_list->moves[move_list->count] = Move(NORMAL, from, to, PAWN);
			move_list->count++;

		}

        }

	
	while (ep_squares != 0ULL){	
		int from = utils::pop_lsb(ep_squares);

		move_list->moves[move_list->count] = Move(EN_PASSANT_FLAG, from, enPassantSquare - 8, PAWN, PAWN);
		move_list->count++;		

	}	

}
void MoveGen::generateRookMoves(bool side, uint64_t rookbb, uint64_t myBlockers, uint64_t oppBlockers, MoveList* move_list){
	uint64_t all = oppBlockers | myBlockers;

        while (rookbb != 0ULL){
                int from = utils::pop_lsb(rookbb);


                int index = utils::generateMagicIndex( all & rookOccupancyMasks[from], rookMagics[from], from, 0);

		uint64_t legalMove = rookMoveList[from][index] & ~(myBlockers); 

                addPossibleMove(from, legalMove, side, ROOK, move_list);

        }


}
void MoveGen::generateBishopMoves(bool side, uint64_t bishopbb, uint64_t myBlockers, uint64_t oppBlockers, MoveList* move_list){
	uint64_t all = myBlockers | oppBlockers;

        while (bishopbb != 0ULL){

                int from = utils::pop_lsb(bishopbb);


                int index = utils::generateMagicIndex((all) & bishopOccupancyMasks[from], bishopMagics[from], from, 1);

		uint64_t legalMove = bishopMoveList[from][index] & ~(myBlockers); 

                addPossibleMove(from, legalMove, side, BISHOP, move_list);
        }


}

void MoveGen::generateQueenMoves(bool side, uint64_t queenbb, uint64_t myBlockers, uint64_t oppBlockers, MoveList* move_list){
	uint64_t all = myBlockers | oppBlockers;

        while (queenbb != 0ULL){
                int from = utils::pop_lsb(queenbb);

                int rookIndex = utils::generateMagicIndex((all) & rookOccupancyMasks[from], rookMagics[from], from, 0);

                int bishopIndex = utils::generateMagicIndex((all) & bishopOccupancyMasks[from], bishopMagics[from], from, 1);


		uint64_t legalBishopMoves = bishopMoveList[from][bishopIndex] & ~(myBlockers);
                uint64_t legalRookMoves = rookMoveList[from][rookIndex] & ~(myBlockers);

		uint64_t legalQueenMoves = legalRookMoves | legalBishopMoves;
                addPossibleMove(from, (legalQueenMoves) , side, QUEEN, move_list);

        }

}

void MoveGen::addKingSideCastling(bool side, MoveList* move_list){
        Move castlingMove;
	(side == WHITE) ? castlingMove = Move(W_KS_CASTLE_FLAG, E1, G1, KING) : castlingMove = Move(B_KS_CASTLE_FLAG, E8, G8, KING);
	move_list->moves[move_list->count] = castlingMove;
	move_list->count++;
}

void MoveGen::addQueenSideCastling(bool side, MoveList* move_list){
        Move castlingMove;
        (side == WHITE) ? castlingMove = Move(W_QS_CASTLE_FLAG, E1, C1, KING) : castlingMove = Move(B_QS_CASTLE_FLAG, E8, C8, KING);
	move_list->moves[move_list->count] = castlingMove;
	move_list->count++;
}
