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
		
			enPassantWhiteSquare = 0;
			enPassantBlackSquare = 0;
	
			enPassantWhiteFlag = false;
			enPassantBlackFlag = false;	
				
			setMoves();	
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
	
			enPassantWhiteSquare = 0;
			enPassantBlackSquare = 0;

			enPassantWhiteFlag = false;
			enPassantBlackFlag = false;
			setMoves();
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
	
	std::string moveMessage = "Invalid move " + pieceSquareNames[from] + pieceSquareNames[to];	
	if (side != turn) return "It is not your turn!";

	
	if (side == WHITE && enPassantWhiteFlag) 
		pieces.addEnPassantRights(WHITE, enPassantWhiteSquare);
	if (side == BLACK && enPassantBlackFlag)
		pieces.addEnPassantRights(BLACK, enPassantBlackSquare);
		
	addCastlingRights();
	for (size_t i = 0; i < PIECE_TYPES; i++){
			
		if ((pieces.getPiecesBB(side, i) & bitset(from)) && (pieces.getMovesBB(side, i) & bitset(to))){
			if (side == WHITE && i == KING && from == E1 && to == G1)
			{

				special = CASTLE_FLAG;	

				
				uint64_t kingBB = pieces.getPiecesBB(WHITE,KING);
				uint64_t rookBB = pieces.getPiecesBB(WHITE, ROOK);

				pieces.setAnyPosBB( WHITE, KING, bitclear(kingBB, from));	
				pieces.setAnyPosBB( WHITE, KING, pieces.getPiecesBB(WHITE, KING) | bitset(to));
					
				pieces.setAnyPosBB( WHITE, ROOK, bitclear(rookBB , H1));
				pieces.setAnyPosBB( WHITE, ROOK,pieces.getPiecesBB(WHITE, ROOK) | bitset(F1));
			}

			else if (side == WHITE && i == KING && from == E1 && to == C1){
			
				special = CASTLE_FLAG;
				
				uint64_t kingBB = pieces.getPiecesBB(WHITE,KING);
				uint64_t rookBB = pieces.getPiecesBB(WHITE, ROOK);
				
				pieces.setAnyPosBB( WHITE, KING , bitclear(kingBB, from));
				pieces.setAnyPosBB( WHITE, KING ,pieces.getPiecesBB(WHITE, KING) | bitset(to));
				
				pieces.setAnyPosBB( WHITE, ROOK , bitclear(rookBB, A1));
				pieces.setAnyPosBB( WHITE, ROOK ,pieces.getPiecesBB(WHITE, ROOK) | bitset(D1));
			}

			else if ( side == BLACK && i == KING && from == E8 && to == G8){

				special = CASTLE_FLAG;


                                uint64_t kingBB = pieces.getPiecesBB(BLACK,KING);
                                uint64_t rookBB = pieces.getPiecesBB(BLACK, ROOK);

                                pieces.setAnyPosBB( BLACK, KING, bitclear(kingBB, from));
                                pieces.setAnyPosBB( BLACK, KING, pieces.getPiecesBB(BLACK, KING) | bitset(to));

                                pieces.setAnyPosBB( BLACK, ROOK, bitclear(rookBB , H8));
                                pieces.setAnyPosBB( BLACK, ROOK,pieces.getPiecesBB(BLACK, ROOK) | bitset(F8));
			}

			else if ( side == BLACK && i == KING && from == E8 && to == C8){

				special = CASTLE_FLAG;

                                uint64_t kingBB = pieces.getPiecesBB(BLACK, KING);
                                uint64_t rookBB = pieces.getPiecesBB(BLACK, ROOK);

                                pieces.setAnyPosBB( BLACK, KING , bitclear(kingBB, from));
                                pieces.setAnyPosBB( BLACK, KING ,pieces.getPiecesBB(BLACK, KING) ^ bitset(to));

                                pieces.setAnyPosBB( BLACK, ROOK , bitclear(rookBB, A8));
                                pieces.setAnyPosBB( BLACK, ROOK ,pieces.getPiecesBB(BLACK, ROOK) ^ bitset(D8));
			}

			else if (i == PAWN && pieces.isPromoting(side, from, to))
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

				validCapture = true;
				
			}

		else if ( i == PAWN && side == WHITE && (bitset(to) & bitset(enPassantWhiteSquare<<8))){
			special = EN_PASSANT_FLAG;
			
			uint64_t whitePawnBB = pieces.getPiecesBB(WHITE, PAWN);
			uint64_t blackPawnBB = pieces.getPiecesBB(BLACK, PAWN);
			
			pieces.setAnyPosBB(WHITE, PAWN, bitclear(whitePawnBB, from));
			pieces.setAnyPosBB(WHITE, PAWN, whitePawnBB ^ bitset(to));
			pieces.setAnyPosBB(BLACK, PAWN, bitclear(blackPawnBB, enPassantWhiteSquare));
			

			enPassantWhiteFlag = false;
			enPassantWhiteSquare = 0;
		}


		else if ( i == PAWN && side == BLACK && (bitset(to) & bitset(enPassantBlackSquare>>8))){
			special = EN_PASSANT_FLAG;	
			
			uint64_t whitePawnBB = pieces.getPiecesBB(WHITE, PAWN);
			uint64_t blackPawnBB = pieces.getPiecesBB(BLACK, PAWN);
			
			pieces.setAnyPosBB(BLACK, PAWN, bitclear(blackPawnBB, from));
			pieces.setAnyPosBB(BLACK, PAWN, blackPawnBB ^ bitset(to));
			pieces.setAnyPosBB(WHITE, PAWN, bitclear(whitePawnBB, enPassantBlackSquare));
			

			enPassantBlackFlag = false;
			enPassantBlackSquare = 0;
		}


		else {
				

		if (side == BLACK && movePawnFifthRank(from, to)){	
			enPassantWhiteFlag = true;
			enPassantWhiteSquare = to;
		}

		else if (side == WHITE && movePawnFourthRank(from, to) ){
			enPassantBlackFlag = true;
			enPassantBlackSquare = to;
		}
			
		
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
			
			
                         moveMessage = pieceSquareNames[from] + pieceSquareNames[to];
		}
	


		

			
			

	}

	
	for (size_t i = 0; i < PIECE_TYPES && validCapture; i++){
		if (pieces.getPiecesBB(!side, i) & bitset(to)){ 
			special = CAPTURE_FLAG;
			uint64_t capturedBB = pieces.getPiecesBB(!side, i);
                        pieces.setAnyPosBB( !side, i ,bitclear(capturedBB, to));

		}

	}

	pieces.setSidePiecesBB(side);
	
	if(moveMessage.find("Invalid move") != std::string::npos)
		return moveMessage;
	if (side == WHITE){
		enPassantWhiteFlag = false;
		enPassantWhiteSquare = 0;
	}
	else {
		enPassantBlackFlag = false;
		enPassantBlackSquare = 0;
	}


	addMoveToHistory(utils::encodeMove(from, to, special, 0));	
	
	turn = !turn;	
	pieces.clearMoves(side);
	pieces.generateAllMoves(!side);
	setMoves();
	
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

bool Board::movePawnFifthRank(int from, int to){
	
	return (pieces.getPiecesBB(BLACK, PAWN) & RANK_7 & bitset(from)) && (pieces.getMovesBB(BLACK, PAWN) & RANK_5 & bitset(to)) && (pieces.getPiecesBB(WHITE, PAWN) & (bitset(to<<1) | bitset(to>>1)));

}

bool Board::movePawnFourthRank(int from, int to){

	return (( pieces.getPiecesBB(BLACK, PAWN) &(bitset(to<<1) | bitset(to>>1))) && pieces.getPiecesBB(WHITE, PAWN) & RANK_2 & bitset(from)) && (pieces.getMovesBB(WHITE, PAWN) & RANK_4 & bitset(to));

}

void Board::addMoveToHistory(int move){
	actualMoves[actualMoveCount] = move;
	actualMoveCount++;
}
