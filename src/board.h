#include <iostream>
#include "pieces.h"
#include "types.h"
#include <cctype>
#define PIECE_TYPES 7

class Board{
	private:
		PieceManager pieces;
		array<unsigned int, 256> generatedMoves;
		int actualMoves[256];
		int moveCount;
		bool turn;
		bool hasMovedWhiteKing;
		bool hasMovedA1Rook;
		bool hasMovedH1Rook;
		bool hasMovedBlackKing;
		bool hasMovedA8Rook;
		bool hasMovedH8Rook;
	public:
		Board(){
			pieces = PieceManager();
			moveCount = 0;
			turn = false;
			hasMovedWhiteKing = false;
			hasMovedA1Rook = false;
			hasMovedH1Rook = false;
			hasMovedBlackKing = false;
			hasMovedA8Rook = false;
			hasMovedH8Rook = false;
		
			generatedMoves = getMove();	
			addCastlingRights();	
		}

		Board(PieceArgs args){
			pieces = PieceManager(args);
			
			moveCount = 0;
			turn = false;
			hasMovedWhiteKing = false;
			hasMovedA1Rook = false;
			hasMovedH1Rook = false;
			hasMovedBlackKing = false;
			hasMovedA8Rook = false;
			hasMovedH8Rook = false;
		
			generatedMoves = getMove();	
			addCastlingRights();
		}
		array<unsigned int, 256> getMove() {
			return pieces.getEncodedMove();
		}
		void addEncodedMove();
		void getGeneratedEncodedMoves();
		void generateEncodedMoves();	
		std::string makeMove(int, int, bool);	
		void unmakeMove();
		void printBoard(void);
		void addCastlingRights(void);
};

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
std::string Board::makeMove(int from, int to, bool side){
	bool validCapture = false;
	std::string moveMessage = "Invalid move " + std::to_string(from) + std::to_string(to);	
	if (side != turn) return "It is not your turn!";

	addCastlingRights();	
	for (int i = 0; i < PIECE_TYPES - 1; i++){
			
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

		if (validCapture && (pieces.getMovesBB(side, i) & bitset(from)))
			pieces.setAnyPosBB( side, i , pieces.getPiecesBB(side, i) | bitset(to));

	}

	pieces.setSidePiecesBB(side);
		
	turn = !turn;	
	pieces.clearMoves(side);
	pieces.generateAllMoves(!side);
	return moveMessage;		
}

void Board::printBoard(){
	array<char, 64> board;

	fill(begin(board), end(board), '.');
	
	array<char, 6> char_type = {'k', 'r', 'p', 'b', 'q', 'n'};
	for (int square = 0; square < 64; square++){
		for (int side = 0; side < 2; side++){
		for(int type = 0; type < PIECE_TYPES - 1; type++){
			
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
