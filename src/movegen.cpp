#include <iostream>
#include "magic.h"
#include "move.h"
#include "pieces.h"
#include "types.h"
#include "utils.h"
#include "movegen.h"

void MoveGen::generateMoves(bool side, const PieceBB* friendly, uint64_t all_enemy, MoveList* move_list, int enPassantSquare, bool canWhiteKSCastle, bool canWhiteQSCastle, bool canBlackKSCastle, bool canBlackQSCastle){
	uint64_t all_friendly = friendly->king_bb | friendly->knight_bb | friendly->pawn_bb | friendly->rook_bb | friendly->bishop_bb | friendly->queen_bb;	
	generateKingMoves(side, friendly->king_bb, all_friendly, move_list, canWhiteKSCastle, canWhiteQSCastle, canBlackKSCastle, canBlackQSCastle);
	generateKnightMoves(side, friendly->knight_bb, all_friendly, move_list);
	
	(side == WHITE) ? generateWhitePawnMoves(side, friendly->pawn_bb, all_friendly, all_enemy, move_list, enPassantSquare) : generateBlackPawnMoves(side, friendly->pawn_bb, all_friendly, all_enemy, move_list, enPassantSquare);

	generateRookMoves(side, friendly->rook_bb, all_friendly, all_enemy, move_list);
	generateBishopMoves(side, friendly->bishop_bb, all_friendly, all_enemy, move_list);
	generateQueenMoves(side, friendly->queen_bb, all_friendly, all_enemy, move_list);

}



void MoveGen::addPossibleMove(Square start, uint64_t positions, bool side, PieceType type, MoveList* move_list){

	
	while (positions != 0){
                Square to = utils::pop_lsb(positions);
		move_list->moves[move_list->count] = Move(NORMAL, start, to, type);			// }
		move_list->count++;

        }
           

}
void MoveGen::generateKingMoves(bool side, uint64_t kingbb, uint64_t blockers, MoveList* move_list, bool canWhiteKSCastle, bool canWhiteQSCastle, bool canBlackKSCastle, bool canBlackQSCastle){

        while (kingbb != 0ULL){
                Square from = utils::pop_lsb(kingbb);
		
		uint64_t legalMove = kingLookups[from] & ~(blockers);
                
		addPossibleMove(from, legalMove, side, KING, move_list);
       		 
	}

	if (canWhiteKSCastle)
	{
		move_list->moves[move_list->count] = Move(W_KS_CASTLE_FLAG, E1, G1, KING);
		move_list->count++;
	}

	if (canWhiteQSCastle)
	{
		move_list->moves[move_list->count] = Move(W_QS_CASTLE_FLAG, E1, C1, KING);
		move_list->count++;
	}	

	if (canBlackKSCastle)
	{
		move_list->moves[move_list->count] = Move(B_KS_CASTLE_FLAG, E8, G8, KING);
		move_list->count++;
	}
	if (canBlackQSCastle)
	{
		move_list->moves[move_list->count] = Move(B_QS_CASTLE_FLAG, E8, C8, KING);
		move_list->count++;
	}
}

void MoveGen::generateKnightMoves(bool side, uint64_t knightbb, uint64_t sameSidePieces, MoveList* move_list){

 while (knightbb != 0ULL){
        Square from = utils::pop_lsb(knightbb);

	uint64_t legalMove = knightLookups[from] & ~(sameSidePieces);


        addPossibleMove(from, legalMove, side, KNIGHT, move_list);

 }
}

void MoveGen::generateWhitePawnMoves(bool side, uint64_t pawnbb, uint64_t myBlockers, uint64_t oppBlockers, MoveList* move_list, int enPassantSquare){
	uint64_t all = myBlockers | oppBlockers;
       	uint64_t empty = ~all;
	uint64_t ep_squares = (enPassantSquare == -1) ? 0 : (pawnbb & RANK_5 & ( bitset((enPassantSquare - 1 - 8)) | bitset((enPassantSquare + 1 - 8))));
	uint64_t normal_moves = pawnbb & ~RANK_7;	
       	uint64_t promotions = pawnbb & RANK_7;	

	while (normal_moves != 0ULL){
                Square from = utils::pop_lsb(normal_moves);

		uint64_t single_pawn_pushes = pawnLookups[side][from] & empty;
               
		uint64_t shift = single_pawn_pushes<<8;
		uint64_t double_pawn_pushes = shift & empty & RANK_4;


                uint64_t legalMove = single_pawn_pushes | double_pawn_pushes  | (pawnAttackLookups[side][from] & oppBlockers );
                
		while ( legalMove != 0ULL){
			Square to = utils::pop_lsb(legalMove);
			move_list->moves[move_list->count] = Move(NORMAL, from, to, PAWN);
			move_list->count++;

		}

        }


	while (ep_squares != 0ULL){	
		Square from = utils::pop_lsb(ep_squares);
		Square ep_square = static_cast<Square>(enPassantSquare);

		move_list->moves[move_list->count] = Move(EN_PASSANT_FLAG, from, ep_square , PAWN, PAWN);
		move_list->count++;

	}

	
	while (promotions != 0ULL){
		Square from = utils::pop_lsb(promotions);
		

		
		uint64_t normal_promotions = pawnLookups[side][from] & empty;
		while (normal_promotions != 0ULL){
			Square to = utils::pop_lsb(normal_promotions);
			for(auto& normal_promoted: normal_promoted_codes){
				move_list->moves[move_list->count] = Move(normal_promoted, from, to, PAWN);
				move_list->count++;
			}
		}

		uint64_t captures = pawnAttackLookups[side][from] & oppBlockers;
		
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

void MoveGen::generateBlackPawnMoves(bool side, uint64_t pawnbb, uint64_t myBlockers, uint64_t oppBlockers, MoveList* move_list, int enPassantSquare){
 	uint64_t all = myBlockers | oppBlockers;
        uint64_t empty = ~all;
	uint64_t ep_squares = (enPassantSquare == -1) ? 0: pawnbb & RANK_4 & ( bitset((enPassantSquare - 1 + 8)) | bitset((enPassantSquare + 1 + 8))) ;
      
        uint64_t normal_moves = pawnbb & ~RANK_2;	
       	uint64_t promotions = pawnbb & RANK_2;	
	
	//single and double pawn pushes
	//
	while (normal_moves != 0ULL){
                Square from = utils::pop_lsb(normal_moves);

                uint64_t single_pawn_pushes = pawnLookups[side][from] & empty;

                uint64_t shift = single_pawn_pushes>>8;
                uint64_t double_pawn_pushes = shift & empty & RANK_5;


                uint64_t legalMove = single_pawn_pushes | double_pawn_pushes  | (pawnAttackLookups[side][from] & oppBlockers );

		while ( legalMove != 0ULL){
			Square to = utils::pop_lsb(legalMove);
			move_list->moves[move_list->count] = Move(NORMAL, from, to, PAWN);
			move_list->count++;

		}

        }

	// en passant	
	while (ep_squares != 0ULL){	
		Square from = utils::pop_lsb(ep_squares);
		Square ep_square = static_cast<Square>(enPassantSquare);

		move_list->moves[move_list->count] = Move(EN_PASSANT_FLAG, from, ep_square, PAWN, PAWN);
		move_list->count++;		

	}	


	while (promotions != 0ULL){
		Square from = utils::pop_lsb(promotions);
		

		uint64_t normal_promotions = pawnLookups[side][from] & empty;
		while (normal_promotions != 0ULL){
			Square to = utils::pop_lsb(normal_promotions);
			for(auto& normal_promoted: normal_promoted_codes){
				move_list->moves[move_list->count] = Move(normal_promoted, from, to, PAWN);
				move_list->count++;
			}
		}

		uint64_t captures = pawnAttackLookups[side][from] & oppBlockers;
		
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
void MoveGen::generateRookMoves(bool side, uint64_t rookbb, uint64_t myBlockers, uint64_t oppBlockers, MoveList* move_list){
	uint64_t all = oppBlockers | myBlockers;

        
	while (rookbb != 0ULL){
                Square from = utils::pop_lsb(rookbb);


                int index = utils::generateMagicIndex( all & rookOccupancyMasks[from], rookMagics[from], from, 0);
	
		uint64_t legalMove = rookMoveList[from][index] & ~(myBlockers); 

		addPossibleMove(from, legalMove, side, ROOK, move_list);

        }
	
}
void MoveGen::generateBishopMoves(bool side, uint64_t bishopbb, uint64_t myBlockers, uint64_t oppBlockers, MoveList* move_list){
	uint64_t all = myBlockers | oppBlockers;

        while (bishopbb != 0ULL){

                Square from = utils::pop_lsb(bishopbb);


                int index = utils::generateMagicIndex((all) & bishopOccupancyMasks[from], bishopMagics[from], from, 1);

		uint64_t legalMove = bishopMoveList[from][index] & ~(myBlockers); 

                addPossibleMove(from, legalMove, side, BISHOP, move_list);

        }

}

void MoveGen::generateQueenMoves(bool side, uint64_t queenbb, uint64_t myBlockers, uint64_t oppBlockers, MoveList* move_list){
	uint64_t all = myBlockers | oppBlockers;

        while (queenbb != 0ULL){
                Square from = utils::pop_lsb(queenbb);

                int rookIndex = utils::generateMagicIndex((all) & rookOccupancyMasks[from], rookMagics[from], from, 0);

                int bishopIndex = utils::generateMagicIndex((all) & bishopOccupancyMasks[from], bishopMagics[from], from, 1);


		uint64_t legalBishopMoves = bishopMoveList[from][bishopIndex] & ~(myBlockers);
                uint64_t legalRookMoves = rookMoveList[from][rookIndex] & ~(myBlockers);

		uint64_t legalQueenMoves = legalRookMoves | legalBishopMoves;
                addPossibleMove(from, (legalQueenMoves) , side, QUEEN, move_list);

        }

}

