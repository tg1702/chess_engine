#include "board.h"
#include "movegen.h"
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>



Board::Board(){
			turn = WHITE;
		 
			canWhiteQSCastle = true;
			canWhiteKSCastle = true;
			canBlackQSCastle = true;
			canBlackKSCastle = true;	
			
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
	
	canWhiteKSCastle = false;
	canWhiteQSCastle = false;
	canBlackKSCastle = false;
	canBlackQSCastle = false;

	if (fen != "-") 
	{
		
		for(const char& f: fen){
			if (f == 'K')
				canWhiteKSCastle = true;
			if (f == 'Q')
				canWhiteQSCastle = true;
			if (f == 'k')
				canBlackKSCastle = true;
			if (f == 'q')
				canBlackQSCastle = true;
		}

	}
	
	castlingRights[0][actualMoveCount] = canWhiteKSCastle;
	castlingRights[1][actualMoveCount] = canWhiteQSCastle;
	castlingRights[2][actualMoveCount] = canBlackKSCastle;
	castlingRights[3][actualMoveCount] = canBlackQSCastle;
}

void Board::parseHalfMoveClock(std::string &fen){

}

void Board::parseFullMoveClock(std::string &fen){

}


Board::Board(std::string fen){
	setFEN(fen);		
}



void Board::setFEN(std::string fen){

	std::vector<std::string> splitFen = utils::split_string(fen);
	
	if (splitFen.size() != 6) return;
	
	parsePieceFen(splitFen[0]);
	parseTurn(splitFen[1]);
	parseCastlingRights(splitFen[2]);
	parseEnPassantSquares(splitFen[3]);
	parseHalfMoveClock(splitFen[4]);
	parseFullMoveClock(splitFen[5]);
}
void Board::makeMove(Move& m){
	makeMoveHelper(m);
	turn = !turn;
}

void Board::makeMoveHelper(Move& m){
	
	enPassantSquare = -1;
	
	Square from = m.getFrom();
	Square to = m.getTo();
	int special = m.getFlag();

	bool validCapture = false;
	
	PieceType pieceType = m.getFromPiece();
	PieceType toPieceType = KING;
	PieceType capturedPieceType = KING;

	
	std::string moveMessage = "Invalid move " + pieceSquareNames[from] + pieceSquareNames[to];	

			if (special == NORMAL) {

				if (turn == BLACK && pieceType == PAWN && movePawnFifthRank(from, to)){
				
					enPassantSquare = to + 8;
				}

				else if (turn == WHITE && pieceType == PAWN && movePawnFourthRank(from, to)){
					enPassantSquare = to - 8;
				}
				
	
				pieces.movePiece(turn, pieceType, from, to);
				validCapture = true;
			}

			else if (special == W_KS_CASTLE_FLAG)
			{

				whiteKingSideCastle();	
			}

			else if (special == W_QS_CASTLE_FLAG){
			
				whiteQueenSideCastle();	
			}

			else if (special == B_KS_CASTLE_FLAG){

				blackKingSideCastle();
			}

			else if (special == B_QS_CASTLE_FLAG){

				blackQueenSideCastle();
			}

			else if (special == QUEEN_PROMOTION){
				pieces.addPiece(turn, QUEEN, to);
				pieces.clearPiece(turn, PAWN, from);	
			}
			else if (special == ROOK_PROMOTION){
                		pieces.addPiece(turn, ROOK, to);
                		pieces.clearPiece(turn, PAWN, from);
        		}
			else if (special == BISHOP_PROMOTION){
                		pieces.addPiece(turn, BISHOP, to);
                		pieces.clearPiece(turn, PAWN, from);
        		}
			else if (special == KNIGHT_PROMOTION){
                		pieces.addPiece(turn, KNIGHT, to);
                		pieces.clearPiece(turn, PAWN, from);
        		}
			else if (special == QUEEN_PROMOTION_CAPTURE){
                		pieces.addPiece(turn, QUEEN, to);
                		pieces.clearPiece(turn, PAWN, from);
        			validCapture = true;
			}
        		else if (special == ROOK_PROMOTION_CAPTURE){
                		pieces.addPiece(turn, ROOK, to);
                		pieces.clearPiece(turn, PAWN, from);
				validCapture = true;
        		}
			
			else if (special == BISHOP_PROMOTION_CAPTURE){
                		pieces.addPiece(turn, BISHOP, to);
                		pieces.clearPiece(turn, PAWN, from);
        			validCapture = true;
			}
			
			else if (special == KNIGHT_PROMOTION_CAPTURE){
                		pieces.addPiece(turn, KNIGHT, to);
                		pieces.clearPiece(turn, PAWN, from);
        			validCapture = true;
			}
			else if ( turn == WHITE && special == EN_PASSANT_FLAG){
				enPassantWhite(from, to);
			}


			else if ( turn == BLACK && special == EN_PASSANT_FLAG){
				enPassantBlack(from, to);
			}

		
			toPieceType = pieceType;

	
	if (validCapture){
		for (const auto& p: PieceTypes){
			if ((pieces.getPiecesBB(!turn, p) & bitset(to))){ 			
				if (special == NORMAL) special = CAPTURE_FLAG;
				pieces.clearPiece(!turn, p , to);
				capturedPieceType = p;
				break;
			} 

		}
	}
		if (pieceType == KING && turn == WHITE) {canWhiteKSCastle = false; canWhiteQSCastle = false;}
		if (pieceType == KING && turn == BLACK) {canBlackKSCastle = false; canBlackQSCastle = false;}
		if (pieceType == ROOK && (bitset(from) & bitset(A1))	&& turn == WHITE) canWhiteQSCastle = false;
		if (pieceType == ROOK && (bitset(from) & bitset(H1))	&& turn == WHITE) canWhiteKSCastle = false;
			
		if (pieceType == ROOK && (bitset(from) & bitset(A8))	&& turn == BLACK) canBlackQSCastle = false;
		if (pieceType == ROOK && (bitset(from) & bitset(H8))	&& turn == BLACK) canBlackKSCastle = false;			

	Move temp = Move(special, from, to, toPieceType, capturedPieceType);	
	addMoveToHistory(temp);	

	castlingRights[0][actualMoveCount] = canWhiteKSCastle;
	castlingRights[1][actualMoveCount] = canWhiteQSCastle;
	castlingRights[2][actualMoveCount] = canBlackKSCastle;
	castlingRights[3][actualMoveCount] = canBlackQSCastle;
	
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
		.pawn_bb = pieces.getPiecesBB(turn, PAWN),

		.all = pieces.getPiecesBB(turn, ALL)
	};

	BoardState state{
		.pieces = friendly,
		
		.turn = turn,
		.whiteKSCastle = canWhiteKSCastle && !turn && pieces.canKingSideCastle(WHITE),
		.whiteQSCastle = canWhiteQSCastle && !turn && pieces.canQueenSideCastle(WHITE),
		.blackKSCastle = canBlackKSCastle && turn && pieces.canKingSideCastle(BLACK),
		.blackQSCastle = canBlackQSCastle && turn && pieces.canQueenSideCastle(BLACK),
		
		.enPassant = enPassantSquare,

		.enemies = pieces.getPiecesBB(!turn, ALL)
	};
	
	generator.setState(state);
	generator.generateMoves(move_list);

	
}


std::vector<Move> Board::generatePseudoLegalMoves(){
	generateMoves();

	std::vector<Move> pseudoLegalMoves;
	
	int count = move_list->count;

		
	for (int i = 0; i < count; ++i){		
		pseudoLegalMoves.push_back(move_list->moves[i]);
	
	}

	return pseudoLegalMoves;

}
std::vector<Move> Board::generateLegalMoves(){
	generateMoves();

	std::vector<Move> legalMoves;
	
	int count = move_list->count;

	bool originalTurn = turn;

	legalMovesCount = 0;

	for (int i = 0; i < count; ++i){
		makeMove(move_list->moves[i]);
	
			
		if (!isInCheck(originalTurn)){
			legalMoves.push_back(move_list->moves[i]);
			legalMovesCount += 1;
		}
		

		unmakeMove();
	}

	return legalMoves;

}
void Board::printBoard(){
	std::array<char, 64> board;

	std::fill(std::begin(board), std::end(board), '.');
	
	std::array<char, 6> char_type = {'k', 'r', 'e', 'b', 'q', 'n'};
	
	for (const auto& square: Squares){
		for (int side = 0; side < 2; side++){
		for (const auto& type: PieceTypes){
			
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
	std::cout << " turn = " << ((turn) ? "BLACK" : "WHITE") << '\n';
	std::cout << " ep = " << ((enPassantSquare == -1) ? "None" : pieceSquareNames[enPassantSquare]) << '\n';
	std::cout << " white kingside castle " << ((canWhiteKSCastle) ? "y" : "n") << '\n';
	std::cout << " white queenside castle " << ((canWhiteQSCastle) ? "y" : "n") << '\n';
	
	std::cout << " black kingside castle " << ((canBlackKSCastle) ? "y" : "n") << '\n';
	std::cout << " black queenside castle " << ((canBlackQSCastle) ? "y" : "n") << '\n';
	std::cout << "\n\n\n";
}

bool Board::movePawnFifthRank(Square from, Square to){	
	return ((pieces.getPiecesBB(BLACK, PAWN) & RANK_7 & bitset(from)) && (RANK_5 & bitset(to)));
}

bool Board::movePawnFourthRank(Square from, Square to){

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

void Board::unmakeMoveHelper(){

	Move lastMove = actualMoves[actualMoveCount-1];

	actualMoveCount--;

	PieceType piece = lastMove.getFromPiece();
	Square from = lastMove.getFrom();
	Square to = lastMove.getTo();
	PieceType capturedPieceType = lastMove.getToPiece();
	int flag = lastMove.getFlag();	
	
	if (flag == NORMAL){	
		pieces.movePiece(turn, piece, to, from); 	
		
	}
	else if (flag == CAPTURE_FLAG){
	
			
		pieces.addPiece(!turn, capturedPieceType, to);
		pieces.movePiece(turn, piece, to, from);

	}
	else if (flag == EN_PASSANT_FLAG){
		Square target = static_cast<Square>((turn == WHITE) ? to - 8 : to + 8);
	
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
	
	canWhiteKSCastle = castlingRights[0][actualMoveCount];
	canWhiteQSCastle = castlingRights[1][actualMoveCount];
	canBlackKSCastle = castlingRights[2][actualMoveCount];
	canBlackQSCastle = castlingRights[3][actualMoveCount];	

	pieces.setSidePiecesBB(turn);
	pieces.setSidePiecesBB(!turn);
	
	enPassantSquare = -1;
}


void Board::unmakeMove(){
	turn = !turn;
	unmakeMoveHelper();
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


void Board::enPassantWhite(Square from, Square to){
		
		
			pieces.movePiece(WHITE, PAWN, from, to);	
			
			
			Square square = static_cast<Square>(to-8);
			pieces.clearPiece(BLACK, PAWN, square);



}

void Board::enPassantBlack(Square from, Square to){	
			pieces.movePiece(BLACK, PAWN, from, to);

			Square square = static_cast<Square>(to+8);
                        pieces.clearPiece(WHITE, PAWN, square);

}


bool Board::isInCheck(bool side){
	uint64_t king_bb = pieces.getPiecesBB(side, KING);

	Square kingSquare = utils::pop_lsb(king_bb);
	return pieces.isAttacked(side, kingSquare);
}

int Board::getMaterialCount(bool side){
	return __builtin_popcountll(pieces.getPiecesBB(side, QUEEN)) * 9 + __builtin_popcountll(pieces.getPiecesBB(side, ROOK)) * 5 + __builtin_popcountll(pieces.getPiecesBB(side, BISHOP)) * 3 + __builtin_popcountll(pieces.getPiecesBB(side, KNIGHT)) * 3 + __builtin_popcountll(pieces.getPiecesBB(side, PAWN)) * 1; 
}
bool Board::isGameOver(){	
	return isCheckmated(WHITE) || isCheckmated(BLACK);
}

bool Board::isCheckmated(bool side){
	return !legalMovesCount && isInCheck(side); 

}

bool Board::isStalemate(){
	return !legalMovesCount && (!isInCheck(turn) || !isInCheck(!turn));
}
