#include "board.h"
#include <iostream>
#include <algorithm>
#include <array>

Board::Board(){
			moveCount = 0;
			turn = WHITE;
			hasMovedWhiteKing = false;
			hasMovedA1Rook = false;
			hasMovedH1Rook = false;
			hasMovedBlackKing = false;
			hasMovedA8Rook = false;
			hasMovedH8Rook = false;
		
			enPassantSquareWhite = false;
			enPassantSquareBlack = false;
			
			generatedMoves = getMove();	
			addCastlingRights();	
}

Board::Board(PieceArgs args){
			pieces.setBoard(args);
			
			moveCount = 0;
			turn = WHITE;
			hasMovedWhiteKing = false;
			hasMovedA1Rook = false;
			hasMovedH1Rook = false;
			hasMovedBlackKing = false;
			hasMovedA8Rook = false;
			hasMovedH8Rook = false;
		
			generatedMoves = getMove();	
			addCastlingRights();
}
void Board::addCastlingRights(){
	
			if (turn == WHITE){
			
				if (pieces.canKingSideCastle(WHITE) && !hasMovedH1Rook && !hasMovedWhiteKing)
					pieces.addKingSideCastlingRights(WHITE);

				if (pieces.canQueenSideCastle(WHITE) && !hasMovedA1Rook && !hasMovedWhiteKing)
					pieces.addQueenSideCastlingRights(WHITE);
			}


			else {
				if (pieces.canKingSideCastle(BLACK) && !hasMovedH8Rook && !hasMovedBlackKing)
                                	pieces.addKingSideCastlingRights(BLACK);
                       
				if (pieces.canQueenSideCastle(BLACK) && !hasMovedA8Rook && !hasMovedBlackKing)
                                	pieces.addQueenSideCastlingRights(BLACK);			
			}
}
std::string Board::makeMove(int from, int to, bool side, int special){
	bool validCapture = false;
	std::string moveMessage = "Invalid move " + std::to_string(from) + std::to_string(to);	
	if (side != turn) return "It is not your turn!";

	addCastlingRights();
	for (size_t i = 0; i < PIECE_TYPES; i++){
			
		if ( side == WHITE && i == KING && from == E1 && to == G1 && (pieces.getPiecesBB(side, i) & bitset(E1)) && (pieces.getMovesBB(side, i) & bitset(G1))){
				
				
				moveMessage = "Piece type = " + std::to_string(i) + " from = " + std::to_string(from) + " to = " + std::to_string(to);	

				
				uint64_t kingBB = pieces.getPiecesBB(WHITE,KING);
				uint64_t rookBB = pieces.getPiecesBB(WHITE, ROOK);

				pieces.setAnyPosBB( WHITE, KING, bitclear(kingBB, from));	
				pieces.setAnyPosBB( WHITE, KING, pieces.getPiecesBB(WHITE, KING) | bitset(to));
					
				pieces.setAnyPosBB( WHITE, ROOK, bitclear(rookBB , H1));
				pieces.setAnyPosBB( WHITE, ROOK,pieces.getPiecesBB(WHITE, ROOK) | bitset(F1));
			}
			else if (side == WHITE && i == KING && from == E1 && to == C1 &&(pieces.getPiecesBB(side, i) & bitset(E1)) && (pieces.getMovesBB(side, i) & bitset(C1))){
				moveMessage = "Piece type = " + std::to_string(i) + " from = " + std::to_string(from) + " to = " + std::to_string(to);	
				
				uint64_t kingBB = pieces.getPiecesBB(WHITE,KING);
				uint64_t rookBB = pieces.getPiecesBB(WHITE, ROOK);
				
				pieces.setAnyPosBB( WHITE, KING , bitclear(kingBB, from));
				pieces.setAnyPosBB( WHITE, KING ,pieces.getPiecesBB(WHITE, KING) | bitset(to));
				
				pieces.setAnyPosBB( WHITE, ROOK , bitclear(rookBB, A1));
				pieces.setAnyPosBB( WHITE, ROOK ,pieces.getPiecesBB(WHITE, ROOK) | bitset(D1));
				
			}

			else if ( side == BLACK && i == KING && from == E8 && to == G8 && (pieces.getPiecesBB(side, i) & bitset(E8)) && (pieces.getMovesBB(side, i) & bitset(G8))){
				moveMessage = "Piece type = " + std::to_string(i) + " from = " + std::to_string(from) + " to = " + std::to_string(to);


                                uint64_t kingBB = pieces.getPiecesBB(BLACK,KING);
                                uint64_t rookBB = pieces.getPiecesBB(BLACK, ROOK);

                                pieces.setAnyPosBB( BLACK, KING, bitclear(kingBB, from));
                                pieces.setAnyPosBB( BLACK, KING, pieces.getPiecesBB(BLACK, KING) | bitset(to));

                                pieces.setAnyPosBB( BLACK, ROOK, bitclear(rookBB , H8));
                                pieces.setAnyPosBB( BLACK, ROOK,pieces.getPiecesBB(BLACK, ROOK) | bitset(F8));
                        }
                        else if (side == BLACK && i == KING &&  from == E8 && to == C8 && (pieces.getPiecesBB(side, i) & bitset(E8)) && (pieces.getMovesBB(side, i) & bitset(C8))){

                                moveMessage = "Piece type = " + std::to_string(i) + " from = " + std::to_string(from) + " to = " + std::to_string(to);

                                uint64_t kingBB = pieces.getPiecesBB(BLACK, KING);
                                uint64_t rookBB = pieces.getPiecesBB(BLACK, ROOK);

                                pieces.setAnyPosBB( BLACK, KING , bitclear(kingBB, from));
                                pieces.setAnyPosBB( BLACK, KING ,pieces.getPiecesBB(BLACK, KING) ^ bitset(to));

                                pieces.setAnyPosBB( BLACK, ROOK , bitclear(rookBB, A8));
                                pieces.setAnyPosBB( BLACK, ROOK ,pieces.getPiecesBB(BLACK, ROOK) ^ bitset(D8));

                        }
		
			else if ((pieces.getPiecesBB(side, i) & bitset(from)) && (pieces.getMovesBB(side, i) & bitset(to)) && pieces.isPromoting(side, from, to))
			{
				uint64_t pawnBB = pieces.getPiecesBB(side, PAWN);
				pieces.setAnyPosBB(side, PAWN, bitclear(pawnBB, from));

				switch(special){
					case NORMAL:				// Default case is queen
						special = QUEEN_PROMOTION;
						pieces.setAnyPosBB(side, QUEEN, pieces.getPiecesBB(side, QUEEN) ^ bitset(to));
						break;
					case QUEEN_PROMOTION:
						pieces.setAnyPosBB(side, QUEEN, pieces.getPiecesBB(side, QUEEN) ^ bitset(to));
						break;
					case ROOK_PROMOTION:
						pieces.setAnyPosBB(side, ROOK, pieces.getPiecesBB(side, ROOK) ^ bitset(to));
						break;
					case BISHOP_PROMOTION:
						pieces.setAnyPosBB(side, BISHOP, pieces.getPiecesBB(side, BISHOP) ^ bitset(to));
						break;
					case KNIGHT_PROMOTION:
						pieces.setAnyPosBB(side, KNIGHT, pieces.getPiecesBB(side, KNIGHT) ^ bitset(to));
						break;
				}

				moveMessage = "Promoting " + std::to_string(from) + " pawn to " + std::to_string(special);	
				validCapture = true;
				
			}
/*
		else if (side == BLACK && movedPawnSeventhRank()){
			enPassantWhiteFlag = true;
			pieces.addEnPassantRights(WHITE);
		}

		else if (side == WHITE && movedPawnSecondRank()){
			enPassantBlackFlag = true;
			pieces.addEnPassantRights(BLACK);
		}

		else if ( i == PAWN && side == WHITE && (pieces.getPiecesBB(side, i) & bitset(from) & (bitset(enPassantWhiteFlag<<1) | bitset(enPassantWhiteFlag>>1))) && (pieces.getMovesBB(side, i) & bitset(enPassantWhiteFlag<<8))){
			enPassantWhiteFlag = 0;
			
			pieces.setAnyPosBB(WHITE, PAWN, bitclear(pieces.getMovesBB(WHITE, PAWN), from));
			pieces.setAnyPosBB(WHITE, PAWN, pieces.getMovesBB(WHITE, PAWN) ^ bitset(enPassantWhiteFlag<<8));
			pieces.setAnyPosBB(BLACK, PAWN, bitclear(pieces.getMovesBB(BLACK, PAWN), enPassantWhiteFlag));
		}
		
		else if ( i == PAWN && side == BLACK && (pieces.getPiecesBB(side, i) & bitset(from) & (bitset(enPassantWhiteFlag<<1) | bitset(enPassantBlackFlag>>1))) && (pieces.getMovesBB(side, i) & bitset(enPassantBlackFlag<<8))){
			enPassantBlackFlag = 0;
			
			pieces.setAnyPosBB(BLACK, PAWN, bitclear(pieces.getMovesBB(BLACK, PAWN), from));
			pieces.setAnyPosBB(BLACK, PAWN, pieces.getMovesBB(BLACK, PAWN) ^ bitset(enPassantBlackFlag<<8));
			pieces.setAnyPosBB(WHITE, PAWN, bitclear(pieces.getMovesBB(WHITE, PAWN), enPassantBlackFlag));
		}
			*/
		else if ((pieces.getPiecesBB(side, i) & bitset(from)) && (pieces.getMovesBB(side, i) & bitset(to))){
			
			moveMessage = "Piece type = " + std::to_string(i) + " from = " + std::to_string(from) + " to = " + std::to_string(to);	
			

			uint64_t curBB = pieces.getPiecesBB(side, i);
                        pieces.setAnyPosBB( side, i ,bitclear(curBB, from));
                        pieces.setAnyPosBB( side, i ,pieces.getPiecesBB(side, i) | bitset(to));


                        validCapture = true;


			if (i == KING && side == WHITE) hasMovedWhiteKing = true;
			if (i == KING && side == BLACK) hasMovedBlackKing = true;
			if (i == ROOK && (pieces.getPiecesBB(side, ROOK) & bitset(A1))	&& side == WHITE) hasMovedA1Rook = true;
			if (i == ROOK && (pieces.getPiecesBB(side, ROOK) & bitset(H1))	&& side == WHITE) hasMovedH1Rook = true;
			if (i == ROOK && (pieces.getPiecesBB(side, ROOK) & bitset(A8))	&& side == BLACK) hasMovedA8Rook = true;
			if (i == ROOK && (pieces.getPiecesBB(side, ROOK) & bitset(H8))	&& side == BLACK) hasMovedH8Rook = true;
			}



	}

	
	for (size_t i = 0; i < PIECE_TYPES && validCapture; i++){
		if (pieces.getPiecesBB(!side, i) & bitset(to)){ 
			uint64_t capturedBB = pieces.getPiecesBB(!side, i);
                        pieces.setAnyPosBB( !side, i ,bitclear(capturedBB, to));

		}

	}
	pieces.setSidePiecesBB(side);
	
	if (moveMessage.find("Invalid Move") != std::string::npos)
		return moveMessage;

	turn = !turn;	
	pieces.clearMoves(side);
	pieces.generateAllMoves(!side);
	return moveMessage;		
}

void Board::printBoard(){
	array<char, 64> board;

	fill(begin(board), end(board), '.');
	
	array<char, 6> char_type = {'k', 'r', 'p', 'b', 'q', 'n'};
	for (size_t square = 0; square < 64; square++){
		for (int side = 0; side < 2; side++){
		for(int type = 0; type < PIECE_TYPES; type++){
			
			if (pieces.getPiecesBB(side, type) & (1ULL << square)){
				board[square] = char_type[type];
					
				if (side == WHITE) board[square] = toupper(board[square]);

			}
			

		}

	
		}

	}

	for (int row = 7; row >=0 ; row--){
		for (int col = 7; col >= 0; col--){
			cout << board[row*8 + col] << " ";
		}
		cout << endl;
	}
	cout << "\n\n\n";
}

