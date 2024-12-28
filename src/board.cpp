#include "board.h"
#include "move.h"
#include "movegen.h"
#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>

Board::Board(){
			turn = WHITE;
	
			whiteKSKRMoved = false;
			blackKSKRMoved = false;
			whiteQSKRMoved = false;
			blackQSKRMoved = false;	
			 
			enPassantSquare = -1;
	
			

			}

void Board::parsePieceFen(std::string &fen){

	int squareCount = 0;
	PieceArgs args;	
	std::array<int, 8>  nums = {'1', '2', '3', '4', '5', '6', '7', '8'};
	for(int s = fen.length() - 1; s >= 0; s--){
		if (fen[s] == '/')
			continue;

		if (std::find(nums.begin(), nums.end(), fen[s]) != nums.end()){
			int index = std::find(nums.begin(), nums.end(), fen[s]) - nums.begin();
			squareCount += index + 1;	
		}
		else {
			switch(fen[s]){
			case 'k':
				args.b_king_bb |= bitset(squareCount);
				break;
			case 'q':
				args.b_queens_bb |= bitset(squareCount);
				break;
			case 'r':
				args.b_rooks_bb |= bitset(squareCount);
				break;
			case 'b':
				args.b_bishops_bb |= bitset(squareCount);
				break;
			case 'n':
				args.b_knights_bb |= bitset(squareCount);
				break;
			case 'p':
				args.b_pawns_bb |= bitset(squareCount);
				break;
			 case 'K':                                                                       	
				args.w_king_bb |= bitset(squareCount);
				break;                                                          
			 case 'Q':                                                                       	
				args.w_queens_bb |= bitset(squareCount);
				break;                                                          
			 case 'R':                                                                       
				args.w_rooks_bb |= bitset(squareCount);
                                break;
                        case 'B':
				args.w_bishops_bb |= bitset(squareCount);
                                break;
                        case 'N':
                                args.w_knights_bb |= bitset(squareCount);
				break;
                        case 'P':
                                args.w_pawns_bb |= bitset(squareCount);
				break;
		}
		
			squareCount++;			
	
		}	
	}

	pieces.setBoard(args);	
}

void Board::parseTurn(std::string &fen){
	if (fen.length() != 1) return;

	if ( fen[0] == 'w')
		turn = WHITE;
	else if (fen[0] == 'b')
		turn = BLACK;
}

void Board::parseEnPassantSquares(std::string &fen){
	
	if (fen.length() > 2) return;

	if (fen == "-")
		enPassantSquare = -1;
	else if (pieceSquareValues.find(fen) != pieceSquareValues.end())
		enPassantSquare = pieceSquareValues.at(fen);

	
}

void Board::parseCastlingRights(std::string &fen){

}

void Board::parseHalfMoveClock(std::string &fen){

}

void Board::parseFullMoveClock(std::string &fen){

}

std::vector<string> Board::split_fen(std::string &fen){
	std::stringstream ss(fen);
	std::istream_iterator<std::string> begin(ss), end;
	return std::vector<std::string> (begin, end);
}

Board::Board(std::string fen){
	std::vector<string> splitFen = split_fen(fen);
	
	if (splitFen.size() != 6) return;
	
	parsePieceFen(splitFen[0]);
	parseTurn(splitFen[1]);
	parseCastlingRights(splitFen[2]);
	parseEnPassantSquares(splitFen[3]);
	parseHalfMoveClock(splitFen[4]);
	parseFullMoveClock(splitFen[5]);
	
	whiteKSKRMoved = false;
	blackKSKRMoved = false;
	whiteQSKRMoved = false;
	blackQSKRMoved = false;	
}

Board::Board(PieceArgs &args, bool turn, bool whiteKSCastle, bool whiteQSCastle, bool blackKSCastle, bool blackQSCastle, int epSquare, int halfmove, int fullmove){
			pieces.setBoard(args);
			
			turn = WHITE;

			whiteKSKRMoved = false;
			blackKSKRMoved = false;
			whiteQSKRMoved = false;
			blackQSKRMoved = false;	
	
			enPassantSquare = -1;	

			/*
			canWhiteKSCastle = whiteKSCastle && pieces.canKingSideCastle(WHITE);
			canWhiteQSCastle = whiteQSCastle && pieces.canQueenSideCastle(WHITE);
			canBlackKSCastle = blackKSCastle && pieces.canKingSideCastle(BLACK);
			canBlackQSCastle= blackQSCastle && pieces.canQueenSideCastle(BLACK);
*/
}

void Board::makeMove(Move& m){
	makeMoveHelper(m);
	turn = !turn;
}

void Board::makeMoveHelper(Move& m){
	
	enPassantSquare = -1;
	
	int from = m.getFrom();
	int to = m.getTo();
	int special = m.getFlag();

	int pieceType = m.getFromPiece();
	bool validCapture = false;
	size_t toPieceType = 0;
	size_t capturedPieceType = 0;

	
	std::string moveMessage = "Invalid move " + pieceSquareNames[from] + pieceSquareNames[to];	

			if (special == NORMAL) {

				if (turn == BLACK && pieceType == PAWN && movePawnFifthRank(from, to)){
				
					enPassantSquare = to + 8;
				}

				else if (turn == WHITE && pieceType == PAWN && movePawnFourthRank(from, to)){
					enPassantSquare = to - 8;
				}
				



				if (pieceType == KING && turn == WHITE) {whiteQSKRMoved = true; whiteKSKRMoved = true;}
				if (pieceType == KING && turn == BLACK) {blackQSKRMoved = true; blackKSKRMoved = true;}
				if (pieceType == ROOK && (pieces.getPiecesBB(turn, ROOK) & bitset(A1))	&& turn == WHITE) whiteQSKRMoved = true;
				if (pieceType == ROOK && (pieces.getPiecesBB(turn, ROOK) & bitset(H1))	&& turn == WHITE) whiteKSKRMoved = true;
			
				if (pieceType == ROOK && (pieces.getPiecesBB(turn, ROOK) & bitset(A8))	&& turn == BLACK) blackQSKRMoved = true;
				if (pieceType == ROOK && (pieces.getPiecesBB(turn, ROOK) & bitset(H8))	&& turn == BLACK) blackKSKRMoved = true;	

				
				pieces.movePiece(turn, pieceType, from, to);
				validCapture = true;
			}

			else if (pieceType == KING && special == W_KS_CASTLE_FLAG)
			{

				special = W_KS_CASTLE_FLAG;	
				whiteKingSideCastle();	
			}

			else if (pieceType == KING && special == W_QS_CASTLE_FLAG){
			
				special = W_QS_CASTLE_FLAG;
				whiteQueenSideCastle();	
			}

			else if (pieceType == KING && special == B_KS_CASTLE_FLAG){

				special = B_KS_CASTLE_FLAG;
				blackKingSideCastle();
			}

			else if (pieceType == KING && special == B_QS_CASTLE_FLAG){

				special = B_QS_CASTLE_FLAG;
				blackQueenSideCastle();
			}

			else if (pieceType == PAWN && pieces.isPromoting(turn, from, to))
			{
				if (!special) special = QUEEN_PROMOTION;

				promotePawns(turn, from, to, special);
				validCapture = true;
			}

			else if ( pieceType == PAWN && turn == WHITE && special == EN_PASSANT_FLAG){
				enPassantWhite(from, to);
			}


			else if ( pieceType == PAWN && turn == BLACK && special == EN_PASSANT_FLAG){
				enPassantBlack(from, to);
			}


	moveMessage = pieceSquareNames[from] + pieceSquareNames[to];	
	toPieceType = pieceType;

	
	for (size_t i = 0; i < PIECE_TYPES && validCapture; i++){
		if ((pieces.getPiecesBB(!turn, i) & bitset(to))){ 
			special = updateToCaptureFlag(special);
				
			pieces.clearPiece(!turn, i, to);
			capturedPieceType = i;
			break;
		} 

	}

		
	
	Move temp = Move(special, from, to, toPieceType, capturedPieceType);	
	addMoveToHistory(temp);	

	pieces.setSidePiecesBB(turn);
	pieces.setSidePiecesBB(!turn);
	
	
}

void Board::generateMoves(){
	move_list->count = 0;


	PieceBB friendly {
		.king_bb = pieces.getPiecesBB(turn, KING), 
		.queen_bb = pieces.getPiecesBB(turn, QUEEN), 
		.rook_bb = pieces.getPiecesBB(turn, ROOK),  
		.bishop_bb = pieces.getPiecesBB(turn, BISHOP), 
		.knight_bb = pieces.getPiecesBB(turn, KNIGHT), 
		.pawn_bb = pieces.getPiecesBB(turn, PAWN)
	};


	PieceBB enemy {
		.king_bb = pieces.getPiecesBB(!turn, KING), 
		.queen_bb = pieces.getPiecesBB(!turn, QUEEN), 
		.rook_bb = pieces.getPiecesBB(!turn, ROOK),  
		.bishop_bb = pieces.getPiecesBB(!turn, BISHOP), 
		.knight_bb = pieces.getPiecesBB(!turn, KNIGHT), 
		.pawn_bb = pieces.getPiecesBB(!turn, PAWN)
	};

	
	canWhiteKSCastle = (turn == WHITE) && !whiteKSKRMoved && pieces.canKingSideCastle(WHITE);
	canWhiteQSCastle = (turn == WHITE) && !whiteQSKRMoved && pieces.canQueenSideCastle(WHITE);
	canBlackKSCastle = (turn == BLACK) && !blackKSKRMoved && pieces.canKingSideCastle(BLACK);
	canBlackQSCastle = (turn == BLACK) && !blackQSKRMoved && pieces.canQueenSideCastle(BLACK);
	

	generator.generateMoves(turn, &friendly, pieces.getPiecesBB(!turn, ALL), move_list, enPassantSquare, canWhiteKSCastle, canWhiteQSCastle, canBlackKSCastle, canBlackQSCastle);

	
	
}


std::vector<Move> Board::generatePseudoLegalMoves(){
	generateMoves();

	std::vector<Move> pseudoLegalMoves;
	
	int count = move_list->count;

		
	for (int i = 0; i < count; ++i){		
		pseudoLegalMoves.push_back(move_list->moves[i]);
	
		//if (actualMoveCount == 6)
			//std::cout << pieceSquareNames[move_list->moves[i].getFrom()] << pieceSquareNames[move_list->moves[i].getTo()] << '\n';
	}

	return pseudoLegalMoves;

}
std::vector<Move> Board::generateLegalMoves(){
	generateMoves();

	std::vector<Move> legalMoves;
	
	int count = move_list->count;
	
	for (int i = 0; i < count; ++i){
		makeMove(move_list->moves[i]);
	
			
		if (!isInCheck(turn)){
			legalMoves.push_back(move_list->moves[i]);
		}
		

		unmakeMove();
	}

	return legalMoves;

}
void Board::printBoard(){
	std::array<char, 64> board;

	std::fill(std::begin(board), std::end(board), '.');
	
	std::array<char, 6> char_type = {'k', 'r', 'e', 'b', 'q', 'n'};
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
			std::cout << board[row*8 + col] << " ";
		}
		std::cout << '\n';
	}
	std::cout << " turn = " << turn << '\n';
	std::cout << " ep = " << enPassantSquare << '\n';
	std::cout << "\n\n\n";
}

bool Board::movePawnFifthRank(int from, int to){	
	return ((pieces.getPiecesBB(BLACK, PAWN) & RANK_7 & bitset(from)) && (RANK_5 & bitset(to)));
}

bool Board::movePawnFourthRank(int from, int to){

	return (pieces.getPiecesBB(WHITE, PAWN) & RANK_2 & bitset(from)) && (RANK_4 & bitset(to));

}

void Board::addMoveToHistory(Move& move){	
	actualMoves[actualMoveCount] = move;

	actualMoveCount++;

}

void Board::printHistory(){
}

int Board::getActualMoveCount(){
	return actualMoveCount;
}
void Board::setActualMoveCount(int depth){
}

void Board::unmakeMoveHelper(){

	Move lastMove = actualMoves[actualMoveCount-1];

	actualMoveCount--;

	int piece = lastMove.getFromPiece();
	int from = lastMove.getFrom();
	int to = lastMove.getTo();
	int capturedPieceType = lastMove.getToPiece();
	int flag = lastMove.getFlag();	
	
	if (flag == NORMAL){	
		pieces.movePiece(turn, piece, to, from); 	
		
	}
	else if (flag == CAPTURE_FLAG){
	
			
		pieces.addPiece(!turn, capturedPieceType, to);
		pieces.movePiece(turn, piece, to, from);

	}
	else if (flag == EN_PASSANT_FLAG){
		int target = (turn == WHITE) ? to - 8 : to + 8;
	
		pieces.movePiece(turn, PAWN, to, from);
		pieces.addPiece(!turn, PAWN, target);
	}	
	else if (flag == W_KS_CASTLE_FLAG){

		pieces.movePiece(WHITE, KING, G1, E1);
		pieces.movePiece(WHITE, ROOK, F1, H1); 
	}
	else if (flag == B_KS_CASTLE_FLAG){
		
		pieces.movePiece(BLACK, KING, G8, E8);
		pieces.movePiece(BLACK, ROOK, F8, H8); 
		
	}	
 	else if (flag == W_QS_CASTLE_FLAG){

                pieces.movePiece(WHITE, KING, C1, E1);
                pieces.movePiece(WHITE, ROOK, D1, A1);
        	
	}
        else if (flag == B_QS_CASTLE_FLAG){

                pieces.movePiece(BLACK, KING, C8, E8);
                pieces.movePiece(BLACK, ROOK, D8, A8);
        	
	}
	else if (flag == QUEEN_PROMOTION){
		pieces.clearPiece(turn, QUEEN, to);
		pieces.addPiece(turn, PAWN, from);	
	}
	else if (flag == ROOK_PROMOTION){
                pieces.clearPiece(turn, ROOK, to);
                pieces.addPiece(turn, PAWN, from);
        }
	else if (flag == BISHOP_PROMOTION){
                pieces.clearPiece(turn, BISHOP, to);
                pieces.addPiece(turn, PAWN, from);
        }
	else if (flag == KNIGHT_PROMOTION){
                pieces.clearPiece(turn, KNIGHT, to);
                pieces.addPiece(turn, PAWN, from);
        }
	else if (flag == QUEEN_PROMOTION_CAPTURE){
                pieces.clearPiece(turn, QUEEN, to);
                pieces.addPiece(turn, PAWN, from);
		pieces.addPiece(!turn, capturedPieceType, to);
        }
        else if (flag == ROOK_PROMOTION_CAPTURE){
                pieces.clearPiece(turn, ROOK, to);
                pieces.addPiece(turn, PAWN, from);
                pieces.addPiece(!turn, capturedPieceType, to);
        }
        else if (flag == BISHOP_PROMOTION_CAPTURE){
                pieces.clearPiece(turn, BISHOP, to);
                pieces.addPiece(turn, PAWN, from);
                pieces.addPiece(!turn, capturedPieceType, to);
        }
        else if (flag == KNIGHT_PROMOTION_CAPTURE){
                pieces.clearPiece(turn, KNIGHT, to);
                pieces.addPiece(turn, PAWN, from);
                pieces.addPiece(!turn, capturedPieceType, to);
        }	
	

	whiteKSKRMoved = false;
	whiteQSKRMoved = false;
	blackKSKRMoved = false;
	blackQSKRMoved = false;

	for (int i = 0; i < actualMoveCount; i++){
		int formerPiece = actualMoves[i].getFromPiece();

		if (formerPiece == KING && (i % 2) == WHITE) {whiteQSKRMoved = true; whiteKSKRMoved = true;}
		if (formerPiece == KING && (i % 2) == BLACK) {blackQSKRMoved = true; blackKSKRMoved = true;}
		if (formerPiece == ROOK &&  (bitset(from) & bitset(A1)) && (i % 2) == WHITE) whiteQSKRMoved = true;
		if (formerPiece == ROOK && (bitset(from) & bitset(H1)) && (i % 2) == WHITE) whiteKSKRMoved = true;	
		if (formerPiece == ROOK && (bitset(from) & bitset(A8)) && (i % 2) == BLACK) blackQSKRMoved = true;
		if (formerPiece == ROOK && (bitset(from) & bitset(H8)) && (i % 2) == BLACK) blackKSKRMoved = true;	
	}

	pieces.setSidePiecesBB(turn);
	pieces.setSidePiecesBB(!turn);

	enPassantSquare = -1;
}


bool Board::isCastlingMove(int from, int to){
	return isWhiteKSCastlingMove(from, to) || isWhiteQSCastlingMove(from, to) || isBlackKSCastlingMove(from, to) || isBlackQSCastlingMove(from, to);  
}

bool Board::isWhiteKSCastlingMove(int from, int to){
	return (turn == WHITE && from == E1 && to == G1); 
}
void Board::unmakeMove(){
	turn = !turn;
	unmakeMoveHelper();
}

bool Board::isWhiteQSCastlingMove(int from, int to){
	return (turn == WHITE && from == E1 && to == C1);
}

bool Board::isBlackKSCastlingMove(int from, int to){
        return (turn == BLACK && from == E8 && to == G8);
}

bool Board::isBlackQSCastlingMove(int from, int to){
        return (turn == BLACK && from == E8 && to == C8);
}

void Board::whiteKingSideCastle(){

	pieces.movePiece(WHITE, KING, E1, G1);
	pieces.movePiece(WHITE, ROOK, H1, F1); 
}

void Board::whiteQueenSideCastle(){

	pieces.movePiece(WHITE, KING, E1, C1);
	pieces.movePiece(WHITE, ROOK, A1, D1); 

}

void Board::blackKingSideCastle(){

	pieces.movePiece(BLACK, KING, E8, G8);
	pieces.movePiece(BLACK, ROOK, H8, F8); 

}

void Board::blackQueenSideCastle(){

	pieces.movePiece(BLACK, KING, E8, C8);
	pieces.movePiece(BLACK, ROOK, A8, D8); 
}

void Board::promotePawns(bool side, int from, int to, int special){
	
				pieces.clearPiece(side, PAWN, from);

				switch(special){
					case QUEEN_PROMOTION:	
						pieces.addPiece(side, QUEEN, to);
						break;
					case ROOK_PROMOTION:
						pieces.addPiece(side, ROOK, to);
						break;
					case BISHOP_PROMOTION:
						pieces.addPiece(side, BISHOP, to);
						break;
					case KNIGHT_PROMOTION:
						pieces.addPiece(side, KNIGHT, to);
						break;

				}

}

void Board::enPassantWhite(int from, int to){
		
		
			pieces.movePiece(WHITE, PAWN, from, to);	
			pieces.clearPiece(BLACK, PAWN, to-8);



}

void Board::enPassantBlack(int from, int to){	
			pieces.movePiece(BLACK, PAWN, from, to);
                        pieces.clearPiece(WHITE, PAWN, to+8);

}


bool Board::isInCheck(bool side){
	int kingSquare = __builtin_ctzll(pieces.getPiecesBB(side, KING));
	return pieces.isAttacked(side, kingSquare);
}

int Board::updateToCaptureFlag(int special){

                                switch(special){
					case NORMAL:
						special = CAPTURE_FLAG;
						break;
					case QUEEN_PROMOTION:
                                                special = QUEEN_PROMOTION_CAPTURE; 
						break;
                                        case ROOK_PROMOTION:
                                                special = ROOK_PROMOTION_CAPTURE;
                                                break;
                                        case BISHOP_PROMOTION:
                                                special = BISHOP_PROMOTION_CAPTURE;
                                                break;
                                        case KNIGHT_PROMOTION:
                                                special = KNIGHT_PROMOTION_CAPTURE;
                                                break;
                                }
	return special;
}
