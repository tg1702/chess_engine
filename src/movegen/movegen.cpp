#include <iostream>
#include <cstdint>
#include <memory>

#include "../movegen/move.h"
#include "../types.h"
#include "../utils.h"
#include "movegen.h"

void MoveGen::setState(BoardState& state){
	this->state = state;
}


void MoveGen::generateMoves(MoveList* move_list){
	generateCaptures(move_list);
	generateQuiets(move_list);

}

void MoveGen::generateCaptures(MoveList* move_list){

	generateKingMoves<MoveType::CAPTURES>(move_list);
	
	generateKnightMoves<MoveType::CAPTURES>(move_list);
	
	(state.turn == Side::WHITE) ? generateWhitePawnMoves<MoveType::CAPTURES>(move_list) : generateBlackPawnMoves<MoveType::CAPTURES>(move_list);

	
	generateRookMoves<MoveType::CAPTURES>(move_list);
	generateBishopMoves<MoveType::CAPTURES>(move_list);
	generateQueenMoves<MoveType::CAPTURES>(move_list);
}

void MoveGen::generateQuiets(MoveList* move_list){

	generateKingMoves<MoveType::QUIETS>(move_list);
	generateKnightMoves<MoveType::QUIETS>(move_list);
	
	(state.turn == Side::WHITE) ? generateWhitePawnMoves<MoveType::QUIETS>(move_list) : generateBlackPawnMoves<MoveType::QUIETS>(move_list);

	generateRookMoves<MoveType::QUIETS>(move_list);
	generateBishopMoves<MoveType::QUIETS>(move_list);
	generateQueenMoves<MoveType::QUIETS>(move_list);
}


template <MoveType Type>
void MoveGen::addPossibleMove(Square start, Bitboard positions, PieceType type, MoveList* move_list){
	
		while (positions != 0){
			PieceType capturedPieceType = KING;
			Flag flag = Flag::NORMAL;
			
			Square to = utils::pop_lsb(positions);

			if (Type == MoveType::CAPTURES){
					for (const auto& p: PieceTypes){
						if (state.enemy_array[p] & bitset(to)){ 
							capturedPieceType = p;
							flag = Flag::CAPTURE;
							break;
						} 
			
					}
					
			}
				
		Move new_move = Move(flag, start, to, type, capturedPieceType);
		
		move_list->moves[move_list->count] = new_move;			// }
		move_list->count++;

    }
           

}

template <MoveType Type>
void MoveGen::generateKingMoves(MoveList* move_list){
		
	
		Bitboard king_bb =  state.pieces.king_bb;
        
		while (king_bb != 0ULL){
            Square from = utils::pop_lsb(king_bb);

			Bitboard legalMove = kingLookups[from] & ~(state.pieces.all);
                
			if (Type == MoveType::CAPTURES)
			{
				addPossibleMove<MoveType::CAPTURES>(from, legalMove & state.enemies, KING, move_list);
			}
				
			else
			{
				addPossibleMove<MoveType::QUIETS>(from, legalMove & ~state.enemies, KING, move_list);	

			}
					 
		}

		if (Type == MoveType::QUIETS){
			if (state.whiteKSCastle)
			{
				move_list->moves[move_list->count] = Move(Flag::W_KS_CASTLE, E1, G1, KING, KING);
				move_list->count++;
			}

			if (state.whiteQSCastle)
			{
				move_list->moves[move_list->count] = Move(Flag::W_QS_CASTLE, E1, C1, KING, KING);
				move_list->count++;
			}	

			if (state.blackKSCastle)
			{
				move_list->moves[move_list->count] = Move(Flag::B_KS_CASTLE, E8, G8, KING, KING);
				move_list->count++;
			}
			if (state.blackQSCastle)
			{
				move_list->moves[move_list->count] = Move(Flag::B_QS_CASTLE, E8, C8, KING, KING);
				move_list->count++;
			}
		}
		
	
}

template <MoveType Type>
void MoveGen::generateKnightMoves(MoveList* move_list){
	Bitboard knight_bb = state.pieces.knight_bb;	
 	
	while (knight_bb != 0ULL){
    	Square from = utils::pop_lsb(knight_bb);

		Bitboard legalMove = knightLookups[from] & ~(state.pieces.all);


        if (Type == MoveType::CAPTURES)
		{
			addPossibleMove<MoveType::CAPTURES>(from, legalMove & state.enemies, KNIGHT, move_list);
		}
			
		else
		{
			addPossibleMove<MoveType::QUIETS>(from, legalMove & ~state.enemies, KNIGHT, move_list);	

		}

 	}
}

template <MoveType Type>
void MoveGen::generateWhitePawnMoves(MoveList* move_list){
	Bitboard all = state.pieces.all | state.enemies;
    Bitboard empty = ~all;
	Bitboard ep_squares = (state.enPassant == -1) ? 0 : (state.pieces.pawn_bb & RANK_5 & ( bitset((state.enPassant - 1 - 8)) | bitset((state.enPassant + 1 - 8))));
	Bitboard normal_moves = state.pieces.pawn_bb & ~RANK_7;	
    Bitboard promotions = state.pieces.pawn_bb & RANK_7;	


		if (Type == MoveType::QUIETS){
			while (normal_moves != 0ULL){
				Square from = utils::pop_lsb(normal_moves);
		
				Bitboard single_pawn_pushes = pawnLookups[Side::WHITE][from] & empty;
					   
				Bitboard shift = single_pawn_pushes<<8;
				Bitboard double_pawn_pushes = shift & empty & RANK_4;
		
		
				Bitboard legalMove = single_pawn_pushes | double_pawn_pushes;
						
				while ( legalMove != 0ULL){
					Square to = utils::pop_lsb(legalMove);
					move_list->moves[move_list->count] = Move(Flag::NORMAL, from, to, PAWN, KING);
					move_list->count++;
		
				}
		
			}
		
			while (promotions != 0ULL){
				Square from = utils::pop_lsb(promotions);
				
				Bitboard normal_promotions = pawnLookups[Side::WHITE][from] & empty;
				while (normal_promotions != 0ULL){
					Square to = utils::pop_lsb(normal_promotions);
					for(auto& normal_promoted: normal_promoted_codes){

						
						move_list->moves[move_list->count] = Move(normal_promoted, from, to, PAWN, KING);
						move_list->count++;
					}
				}
		
				
			}

			
	
		}
		
		else {

			normal_moves = state.pieces.pawn_bb & ~RANK_7;

			while (normal_moves != 0ULL){
				Square from = utils::pop_lsb(normal_moves);
				Bitboard pawn_attack = (pawnAttackLookups[Side::WHITE][from] & state.enemies );

				addPossibleMove<MoveType::CAPTURES>(from, pawn_attack, PAWN, move_list);
				
			}

			while (ep_squares != 0ULL){	
				Square from = utils::pop_lsb(ep_squares);
				Square ep_square = static_cast<Square>(state.enPassant);
	
				move_list->moves[move_list->count] = Move(Flag::EN_PASSANT, from, ep_square , PAWN, PAWN);
				move_list->count++;
	
			}


			promotions = state.pieces.pawn_bb & RANK_7;

			while (promotions != 0ULL){
				Square from = utils::pop_lsb(promotions);
				Bitboard captures = pawnAttackLookups[Side::WHITE][from] & state.enemies;
				
				while (captures != 0ULL)
				{
					Square to = utils::pop_lsb(captures);

					for(auto& capture_promoted: capture_promoted_codes){
						
						PieceType capturedPieceType = KING;
		
						for (const auto& p: PieceTypes){
							if (state.enemy_array[p] & bitset(to)){ 		
								capturedPieceType = p;
								break;
							} 
			
						}
						move_list->moves[move_list->count] = Move(capture_promoted, from, to, PAWN, capturedPieceType);
						move_list->count++;	
				}	
				
			}
		}


	}
}
template <MoveType Type>
void MoveGen::generateBlackPawnMoves(MoveList* move_list){
 	Bitboard all = state.pieces.all | state.enemies;
    Bitboard empty = ~all;
	Bitboard ep_squares = (state.enPassant == -1) ? 0: state.pieces.pawn_bb & RANK_4 & ( bitset((state.enPassant - 1 + 8)) | bitset((state.enPassant + 1 + 8))) ;
      
    Bitboard normal_moves = state.pieces.pawn_bb & ~RANK_2;	
    Bitboard promotions = state.pieces.pawn_bb & RANK_2;	
	
	//single and double pawn pushes
	//

	if (Type == MoveType::QUIETS){
		while (normal_moves != 0ULL){
			Square from = utils::pop_lsb(normal_moves);

			Bitboard single_pawn_pushes = pawnLookups[Side::BLACK][from] & empty;

			Bitboard shift = single_pawn_pushes>>8;
			Bitboard double_pawn_pushes = shift & empty & RANK_5;


			Bitboard legalMove = single_pawn_pushes | double_pawn_pushes;

			while ( legalMove != 0ULL){
				Square to = utils::pop_lsb(legalMove);
				move_list->moves[move_list->count] = Move(Flag::NORMAL, from, to, PAWN, KING);
				move_list->count++;

			}
		}


		while (promotions != 0ULL){
			Square from = utils::pop_lsb(promotions);
			

			Bitboard normal_promotions = pawnLookups[Side::BLACK][from] & empty;
			while (normal_promotions != 0ULL){
				Square to = utils::pop_lsb(normal_promotions);
				for(auto& normal_promoted: normal_promoted_codes){
					move_list->moves[move_list->count] = Move(normal_promoted, from, to, PAWN, KING);
					move_list->count++;
				}
			}

		}

	}
			

	else {
		normal_moves = state.pieces.pawn_bb & ~RANK_2;

		while (normal_moves != 0ULL){
			Square from = utils::pop_lsb(normal_moves);
			Bitboard legalMove = (pawnAttackLookups[Side::BLACK][from] & state.enemies );

			addPossibleMove<MoveType::CAPTURES>(from, legalMove, PAWN, move_list);

		}	

		promotions = state.pieces.pawn_bb & RANK_2;

		while (promotions != 0ULL){
			Square from = utils::pop_lsb(promotions);

			Bitboard captures = pawnAttackLookups[Side::BLACK][from] & state.enemies;
			
			while (captures != 0ULL)
			{
				Square to = utils::pop_lsb(captures);

				for(auto& capture_promoted: capture_promoted_codes){

					PieceType capturedPieceType = KING;
		
						for (const auto& p: PieceTypes){
							if (state.enemy_array[p] & bitset(to)){ 		
								capturedPieceType = p;
								break;
							} 
			
					}	

					move_list->moves[move_list->count] = Move(capture_promoted, from, to, PAWN, capturedPieceType);
					move_list->count++;
				}	
			}	
		}

		while (ep_squares != 0ULL){	
			Square from = utils::pop_lsb(ep_squares);
			Square ep_square = static_cast<Square>(state.enPassant);

			move_list->moves[move_list->count] = Move(Flag::EN_PASSANT, from, ep_square, PAWN, PAWN);
			move_list->count++;		

		}	

	}

	

}

template <MoveType Type>
void MoveGen::generateRookMoves(MoveList* move_list){
	Bitboard all = state.enemies | state.pieces.all;
	Bitboard rook_bb = state.pieces.rook_bb;

        
	while (rook_bb != 0ULL){
        Square from = utils::pop_lsb(rook_bb);


        int index = utils::generateMagicIndex( all & rookOccupancyMasks[from], rookMagics[from], from, 0);
	
		Bitboard legalMove = rookMoveList[from][index] & ~(state.pieces.all); 

		if (Type == MoveType::CAPTURES)
		{
			addPossibleMove<MoveType::CAPTURES>(from, legalMove & state.enemies, ROOK, move_list);
		}
			
		else
		{
			addPossibleMove<MoveType::QUIETS>(from, legalMove & ~state.enemies, ROOK, move_list);	

		}

    }
	
}

template <MoveType Type>
void MoveGen::generateBishopMoves(MoveList* move_list){
	Bitboard all = state.pieces.all | state.enemies;
	Bitboard bishop_bb = state.pieces.bishop_bb;

	while (bishop_bb != 0ULL){

		Square from = utils::pop_lsb(bishop_bb);


		int index = utils::generateMagicIndex((all) & bishopOccupancyMasks[from], bishopMagics[from], from, 1);

		Bitboard legalMove = bishopMoveList[from][index] & ~(state.pieces.all); 

		if (Type == MoveType::CAPTURES)
		{
			addPossibleMove<MoveType::CAPTURES>(from, legalMove & state.enemies, BISHOP, move_list);
		}
				
		else
		{
			addPossibleMove<MoveType::QUIETS>(from, legalMove & ~state.enemies, BISHOP, move_list);	

		}

	}

}

template <MoveType Type>
void MoveGen::generateQueenMoves(MoveList* move_list){
	Bitboard all = state.pieces.all | state.enemies;

		Bitboard queen_bb = state.pieces.queen_bb;

        while (queen_bb != 0ULL){
			Square from = utils::pop_lsb(queen_bb);

			int rookIndex = utils::generateMagicIndex((all) & rookOccupancyMasks[from], rookMagics[from], from, 0);

			int bishopIndex = utils::generateMagicIndex((all) & bishopOccupancyMasks[from], bishopMagics[from], from, 1);


			Bitboard legalBishopMoves = bishopMoveList[from][bishopIndex] & ~(state.pieces.all);
			Bitboard legalRookMoves = rookMoveList[from][rookIndex] & ~(state.pieces.all);

			Bitboard legalQueenMoves = legalRookMoves | legalBishopMoves;
                
			if (Type == MoveType::CAPTURES)
			{
				addPossibleMove<MoveType::CAPTURES>(from, legalQueenMoves & state.enemies, QUEEN, move_list);
			}
				
			else
			{
				addPossibleMove<MoveType::QUIETS>(from, legalQueenMoves & ~state.enemies, QUEEN, move_list);	

			}

        }

}