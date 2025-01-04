#include <iostream>
#include "magic.h"
#include "move.h"
#include "pieces.h"
#include "types.h"
#include "utils.h"
#include "movegen.h"

void MoveGen::setState(BoardState& state){
	this->state = state;
}

void MoveGen::generateMoves(MoveList* move_list){
	
	generateKingMoves(move_list);
	generateKnightMoves(move_list);
	
	(state.turn == WHITE) ? generateWhitePawnMoves(move_list) : generateBlackPawnMoves(move_list);

	generateRookMoves(move_list);
	generateBishopMoves(move_list);
	generateQueenMoves(move_list);

}



void MoveGen::addPossibleMove(Square start, uint64_t positions, bool side, PieceType type, MoveList* move_list){
	
	while (positions != 0){
                Square to = utils::pop_lsb(positions);
		move_list->moves[move_list->count] = Move(NORMAL, start, to, type);			// }
		move_list->count++;

        }
           

}
void MoveGen::generateKingMoves(MoveList* move_list){
		
        while (state.pieces.king_bb != 0ULL){
                Square from = utils::pop_lsb(state.pieces.king_bb);
		
		uint64_t legalMove = kingLookups[from] & ~(state.pieces.all);
                
		addPossibleMove(from, legalMove, state.turn, KING, move_list);
       		 
	}

	if (state.whiteKSCastle)
	{
		move_list->moves[move_list->count] = Move(W_KS_CASTLE_FLAG, E1, G1, KING);
		move_list->count++;
	}

	if (state.whiteQSCastle)
	{
		move_list->moves[move_list->count] = Move(W_QS_CASTLE_FLAG, E1, C1, KING);
		move_list->count++;
	}	

	if (state.blackKSCastle)
	{
		move_list->moves[move_list->count] = Move(B_KS_CASTLE_FLAG, E8, G8, KING);
		move_list->count++;
	}
	if (state.blackQSCastle)
	{
		move_list->moves[move_list->count] = Move(B_QS_CASTLE_FLAG, E8, C8, KING);
		move_list->count++;
	}
}

void MoveGen::generateKnightMoves(MoveList* move_list){

 while (state.pieces.knight_bb != 0ULL){
        Square from = utils::pop_lsb(state.pieces.knight_bb);

		uint64_t legalMove = knightLookups[from] & ~(state.pieces.all);


        addPossibleMove(from, legalMove, state.turn, KNIGHT, move_list);

 }
}

void MoveGen::generateWhitePawnMoves(MoveList* move_list){
	uint64_t all = state.pieces.all | state.enemies;
    uint64_t empty = ~all;
	uint64_t ep_squares = (state.enPassant == -1) ? 0 : (state.pieces.pawn_bb & RANK_5 & ( bitset((state.enPassant - 1 - 8)) | bitset((state.enPassant + 1 - 8))));
	uint64_t normal_moves = state.pieces.pawn_bb & ~RANK_7;	
    uint64_t promotions = state.pieces.pawn_bb & RANK_7;	

	while (normal_moves != 0ULL){
                Square from = utils::pop_lsb(normal_moves);

		uint64_t single_pawn_pushes = pawnLookups[WHITE][from] & empty;
               
		uint64_t shift = single_pawn_pushes<<8;
		uint64_t double_pawn_pushes = shift & empty & RANK_4;


                uint64_t legalMove = single_pawn_pushes | double_pawn_pushes  | (pawnAttackLookups[WHITE][from] & state.enemies );
                
		while ( legalMove != 0ULL){
			Square to = utils::pop_lsb(legalMove);
			move_list->moves[move_list->count] = Move(NORMAL, from, to, PAWN);
			move_list->count++;

		}

        }


	while (ep_squares != 0ULL){	
		Square from = utils::pop_lsb(ep_squares);
		Square ep_square = static_cast<Square>(state.enPassant);

		move_list->moves[move_list->count] = Move(EN_PASSANT_FLAG, from, ep_square , PAWN, PAWN);
		move_list->count++;

	}

	
	while (promotions != 0ULL){
		Square from = utils::pop_lsb(promotions);
		

		
		uint64_t normal_promotions = pawnLookups[WHITE][from] & empty;
		while (normal_promotions != 0ULL){
			Square to = utils::pop_lsb(normal_promotions);
			for(auto& normal_promoted: normal_promoted_codes){
				move_list->moves[move_list->count] = Move(normal_promoted, from, to, PAWN);
				move_list->count++;
			}
		}

		uint64_t captures = pawnAttackLookups[WHITE][from] & state.enemies;
		
		while (captures != 0ULL)
		{
			Square to = utils::pop_lsb(captures);

			for(auto& capture_promoted: capture_promoted_codes){
				move_list->moves[move_list->count] = Move(capture_promoted, from, to, PAWN);
				move_list->count++;
			}	
		}	
	}


}

void MoveGen::generateBlackPawnMoves(MoveList* move_list){
 	uint64_t all = state.pieces.all | state.enemies;
        uint64_t empty = ~all;
	uint64_t ep_squares = (state.enPassant == -1) ? 0: state.pieces.pawn_bb & RANK_4 & ( bitset((state.enPassant - 1 + 8)) | bitset((state.enPassant + 1 + 8))) ;
      
        uint64_t normal_moves = state.pieces.pawn_bb & ~RANK_2;	
       	uint64_t promotions = state.pieces.pawn_bb & RANK_2;	
	
	//single and double pawn pushes
	//
	while (normal_moves != 0ULL){
                Square from = utils::pop_lsb(normal_moves);

                uint64_t single_pawn_pushes = pawnLookups[BLACK][from] & empty;

                uint64_t shift = single_pawn_pushes>>8;
                uint64_t double_pawn_pushes = shift & empty & RANK_5;


                uint64_t legalMove = single_pawn_pushes | double_pawn_pushes  | (pawnAttackLookups[BLACK][from] & state.enemies );

		while ( legalMove != 0ULL){
			Square to = utils::pop_lsb(legalMove);
			move_list->moves[move_list->count] = Move(NORMAL, from, to, PAWN);
			move_list->count++;

		}

        }

	// en passant	
	while (ep_squares != 0ULL){	
		Square from = utils::pop_lsb(ep_squares);
		Square ep_square = static_cast<Square>(state.enPassant);

		move_list->moves[move_list->count] = Move(EN_PASSANT_FLAG, from, ep_square, PAWN, PAWN);
		move_list->count++;		

	}	


	while (promotions != 0ULL){
		Square from = utils::pop_lsb(promotions);
		

		uint64_t normal_promotions = pawnLookups[BLACK][from] & empty;
		while (normal_promotions != 0ULL){
			Square to = utils::pop_lsb(normal_promotions);
			for(auto& normal_promoted: normal_promoted_codes){
				move_list->moves[move_list->count] = Move(normal_promoted, from, to, PAWN);
				move_list->count++;
			}
		}

		uint64_t captures = pawnAttackLookups[BLACK][from] & state.enemies;
		
		while (captures != 0ULL)
		{
			Square to = utils::pop_lsb(captures);

			for(auto& capture_promoted: capture_promoted_codes){
				move_list->moves[move_list->count] = Move(capture_promoted, from, to, PAWN);
				move_list->count++;
			}	
		}	
	}


}
void MoveGen::generateRookMoves(MoveList* move_list){
	uint64_t all = state.enemies | state.pieces.all;

        
	while (state.pieces.rook_bb != 0ULL){
                Square from = utils::pop_lsb(state.pieces.rook_bb);


                int index = utils::generateMagicIndex( all & rookOccupancyMasks[from], rookMagics[from], from, 0);
	
		uint64_t legalMove = rookMoveList[from][index] & ~(state.pieces.all); 

		addPossibleMove(from, legalMove, state.turn, ROOK, move_list);

        }
	
}
void MoveGen::generateBishopMoves(MoveList* move_list){
	uint64_t all = state.pieces.all | state.enemies;

        while (state.pieces.bishop_bb != 0ULL){

                Square from = utils::pop_lsb(state.pieces.bishop_bb);


                int index = utils::generateMagicIndex((all) & bishopOccupancyMasks[from], bishopMagics[from], from, 1);

		uint64_t legalMove = bishopMoveList[from][index] & ~(state.pieces.all); 

                addPossibleMove(from, legalMove, state.turn, BISHOP, move_list);

        }

}

void MoveGen::generateQueenMoves(MoveList* move_list){
	uint64_t all = state.pieces.all | state.enemies;

        while (state.pieces.queen_bb != 0ULL){
                Square from = utils::pop_lsb(state.pieces.queen_bb);

                int rookIndex = utils::generateMagicIndex((all) & rookOccupancyMasks[from], rookMagics[from], from, 0);

                int bishopIndex = utils::generateMagicIndex((all) & bishopOccupancyMasks[from], bishopMagics[from], from, 1);


		uint64_t legalBishopMoves = bishopMoveList[from][bishopIndex] & ~(state.pieces.all);
        uint64_t legalRookMoves = rookMoveList[from][rookIndex] & ~(state.pieces.all);

		uint64_t legalQueenMoves = legalRookMoves | legalBishopMoves;
                addPossibleMove(from, (legalQueenMoves) , state.turn, QUEEN, move_list);

        }

}

