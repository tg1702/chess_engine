#include "board.h"
#include "./movegen/move.h"

#include <cstdint>
#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>
#include <memory>


Board::Board(){
			turn = Side::WHITE;
		 
			canWhiteQSCastle = true;
			canWhiteKSCastle = true;
			canBlackQSCastle = true;
			canBlackKSCastle = true;	
			
			enPassantSquare = -1;
			move_list = new MoveList();	
			
			actualMoveCount = 0;
}

void Board::parsePieceFen(const std::string &fen){

	int squareCount = 0;
	PieceBB w_args{};	
	PieceBB b_args{};	

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
				b_args.king_bb |= bitset(squareCount);
				break;
			case 'q':
				b_args.queen_bb |= bitset(squareCount);
				break;
			case 'r':
				b_args.rook_bb |= bitset(squareCount);
				break;
			case 'b':
				b_args.bishop_bb |= bitset(squareCount);
				break;
			case 'n':
				b_args.knight_bb |= bitset(squareCount);
				break;
			case 'p':
				b_args.pawn_bb |= bitset(squareCount);
				break;
			 case 'K':                                                                       	
				w_args.king_bb |= bitset(squareCount);
				break;                                                          
			 case 'Q':                                                                       	
				w_args.queen_bb |= bitset(squareCount);
				break;                                                          
			 case 'R':                                                                       
				w_args.rook_bb |= bitset(squareCount);
                                break;
                        case 'B':
				w_args.bishop_bb |= bitset(squareCount);
                                break;
                        case 'N':
                                w_args.knight_bb |= bitset(squareCount);
				break;
                        case 'P':
                                w_args.pawn_bb |= bitset(squareCount);
				break;
		}
		
			squareCount++;			
	
		}	
	}

	pieces.setBoard(w_args, b_args);	
}

void Board::parseTurn(const std::string &fen){
	if (fen.length() != 1) return;

	if ( fen[0] == 'w')
		turn = Side::WHITE;
	else if (fen[0] == 'b')
		turn = Side::BLACK;
}

void Board::parseEnPassantSquares(const std::string &fen){
	
	if (fen.length() > 2) return;

	if (fen == "-")
		enPassantSquare = -1;
	else if (pieceSquareValues.find(fen) != pieceSquareValues.end())
		enPassantSquare = pieceSquareValues.at(fen);

	
}

void Board::parseCastlingRights(const std::string &fen){
	
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

void Board::parseHalfMoveClock(const std::string &fen){
	//TODO: store half move clock info
}

void Board::parseFullMoveClock(const std::string &fen){
	//TODO: store full move clock info
}


Board::Board(const std::string fen){

	move_list = new MoveList();	
	setFEN(fen);		
}



void Board::setFEN(const std::string fen){

	actualMoveCount = 0;
	
	std::vector<std::string> splitFen = utils::split_string(fen);
	
	if (splitFen.size() != 6) return;
	
	parsePieceFen(splitFen[0]);
	parseTurn(splitFen[1]);
	parseCastlingRights(splitFen[2]);
	parseEnPassantSquares(splitFen[3]);
	parseHalfMoveClock(splitFen[4]);
	parseFullMoveClock(splitFen[5]);
}
void Board::makeMove(const Move& m){
	makeMoveHelper(m);
	turn = !turn;
}

void Board::makeMove(const std::string& uci_move){
	if (uci_move.length() < 4 || uci_move.length() > 5) return;


	std::string fromString(uci_move.begin(), uci_move.begin() + 2);
	std::string toString(uci_move.begin() + 2, uci_move.begin() + 4);


	if ((pieceSquareValues.find(fromString) != pieceSquareValues.end()) && (pieceSquareValues.find(toString) != pieceSquareValues.end())){
		
		int from = pieceSquareValues.at(fromString);
		int to = pieceSquareValues.at(toString); 	
		
		std::vector<Move> legalMoves = generateLegalMoves();

		
		if (uci_move.length() == 4){
			
			for(auto& move: legalMoves){
				if (move.getFrom() == from && move.getTo() == to){
					makeMove(move);	
					break;
				}
			}
		}
		//dealing with promotions
		else {
		char promotion = uci_move[4];

			for(auto& move: legalMoves){
				if (move.getFrom() == from && move.getTo() == to && 
				   (((move.getFlag() == QUEEN_PROMOTION || move.getFlag() == QUEEN_PROMOTION_CAPTURE) && promotion == 'q') || 
				   ((move.getFlag() == ROOK_PROMOTION || move.getFlag() == ROOK_PROMOTION_CAPTURE) && promotion == 'r') || 
				   ((move.getFlag() == BISHOP_PROMOTION || move.getFlag() == BISHOP_PROMOTION_CAPTURE) && promotion == 'b') || 
				    ((move.getFlag() == KNIGHT_PROMOTION || move.getFlag() == KNIGHT_PROMOTION_CAPTURE) && promotion == 'n'))){
					makeMove(move);	
					break;
				}
			}	
		}

		
	}
}
void Board::makeMoveHelper(const Move& m){
	
	enPassantSquare = -1;
	
	Square from = m.getFrom();
	Square to = m.getTo();
	int special = m.getFlag();

	PieceType pieceType = m.getFromPiece();
	PieceType toPieceType = m.getToPiece();
	

			if (special == NORMAL || special == CAPTURE_FLAG) {

				if (turn == Side::BLACK && pieceType == PAWN && movePawnFifthRank(from, to)){
				
					enPassantSquare = to + 8;
				}

				else if (turn == Side::WHITE && pieceType == PAWN && movePawnFourthRank(from, to)){
					enPassantSquare = to - 8;
				}
				
	
				pieces.movePiece(turn, pieceType, from, to);
				
				if (special == CAPTURE_FLAG) pieces.clearPiece(!turn, toPieceType, to);
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
 
			}
        		else if (special == ROOK_PROMOTION_CAPTURE){
                		pieces.addPiece(turn, ROOK, to);
                		pieces.clearPiece(turn, PAWN, from);
        		}
			
			else if (special == BISHOP_PROMOTION_CAPTURE){
                		pieces.addPiece(turn, BISHOP, to);
                		pieces.clearPiece(turn, PAWN, from);
			}
			
			else if (special == KNIGHT_PROMOTION_CAPTURE){
                		pieces.addPiece(turn, KNIGHT, to);
                		pieces.clearPiece(turn, PAWN, from);

			}
			else if ( turn == Side::WHITE && special == EN_PASSANT_FLAG){
				enPassantWhite(from, to);
			}


			else if ( turn == Side::BLACK && special == EN_PASSANT_FLAG){
				enPassantBlack(from, to);
			}


		if (pieceType == KING && turn == Side::WHITE) {canWhiteKSCastle = false; canWhiteQSCastle = false;}
		if (pieceType == KING && turn == Side::BLACK) {canBlackKSCastle = false; canBlackQSCastle = false;}
		if (pieceType == ROOK && (bitset(from) & bitset(A1))	&& turn == Side::WHITE) canWhiteQSCastle = false;
		if (pieceType == ROOK && (bitset(from) & bitset(H1))	&& turn == Side::WHITE) canWhiteKSCastle = false;
			
		if (pieceType == ROOK && (bitset(from) & bitset(A8))	&& turn == Side::BLACK) canBlackQSCastle = false;
		if (pieceType == ROOK && (bitset(from) & bitset(H8))	&& turn == Side::BLACK) canBlackKSCastle = false;			
	
	addMoveToHistory(m);	

	castlingRights[0][actualMoveCount] = canWhiteKSCastle;
	castlingRights[1][actualMoveCount] = canWhiteQSCastle;
	castlingRights[2][actualMoveCount] = canBlackKSCastle;
	castlingRights[3][actualMoveCount] = canBlackQSCastle;
	
	pieces.setSidePiecesBB(turn);
	pieces.setSidePiecesBB(!turn);

	
}

Bitboard Board::getBitboard(Side turn, PieceType pieceType) const{
	return pieces.getPiecesBB(turn, pieceType);
}

void Board::generateMoves(const MoveType type){
	move_list->count = 0;


	PieceBB friendly {
		pieces.getPiecesBB(turn, KING), 
		pieces.getPiecesBB(turn, QUEEN), 
		pieces.getPiecesBB(turn, ROOK),  
		pieces.getPiecesBB(turn, BISHOP), 
		pieces.getPiecesBB(turn, KNIGHT), 
		pieces.getPiecesBB(turn, PAWN),
		pieces.getPiecesBB(turn, ALL)
	};

	
	BoardState state{
		friendly,
		turn,
		canWhiteKSCastle && !turn && pieces.canKingSideCastle(Side::WHITE),
		canWhiteQSCastle && !turn && pieces.canQueenSideCastle(Side::WHITE),
		canBlackKSCastle && turn && pieces.canKingSideCastle(Side::BLACK),
		canBlackQSCastle && turn && pieces.canQueenSideCastle(Side::BLACK),
		
		enPassantSquare,

		pieces.getPiecesBB(!turn, ALL),


		{
			pieces.getPiecesBB(!turn, PAWN),
			pieces.getPiecesBB(!turn, KNIGHT), 
			pieces.getPiecesBB(!turn, BISHOP), 
			pieces.getPiecesBB(!turn, ROOK), 
			pieces.getPiecesBB(!turn, QUEEN), 
			pieces.getPiecesBB(!turn, KING)
			
		}
	};
	
	generator.setState(state);

	
	//std::cout << " type " << type << '\n';

	if (type == MoveType::QUIETS){
		
		generator.generateQuiets(move_list);

	}
	else if (type == MoveType::CAPTURES){
		generator.generateCaptures(move_list);
		//std::cout << "size " << move_list->count << '\n';
	}
	else if ( type == MoveType::ALL_TYPES){

		generator.generateMoves(move_list);

	}

	
}


std::vector<Move> Board::generatePseudoLegalMoves(MoveType type){
	generateMoves(type);

	std::vector<Move> pseudoLegalMoves;
	
	int count = move_list->count;

		
	for (int i = 0; i < count; ++i){		
		pseudoLegalMoves.push_back(move_list->moves[i]);
	
	}

	return pseudoLegalMoves;

}
std::vector<Move> Board::generateLegalMoves(MoveType type){

	generateMoves(type);

	std::vector<Move> legalMoves;

	int count = move_list->count;

	Side originalTurn = turn;

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
	std::array<char, 64> char_board = {'.'};

	std::fill(std::begin(char_board), std::end(char_board), '.');
	
	std::array<char, 6> char_type = {'p', 'n', 'b', 'r', 'q', 'k'};
	
	for (const auto& square: Squares){
		for (const auto& side: Sides){
		for (const auto& type: PieceTypes){
			
			if (pieces.getPiecesBB(side, type) & (1ULL << square)){
				char_board[square] = char_type[type];
					
				if (side == Side::WHITE) char_board[square] = toupper(char_board[square]);

			}
			

		}

	
		}

	}

	for (int row = 7; row >=0 ; row--){
		for (int col = 7; col >= 0; col--){
			std::cout << char_board[row*8 + col] << " ";
		}
		std::cout << '\n';
	}
	std::cout << " turn = " << ((turn) ? "Side::BLACK" : "Side::WHITE") << '\n';
	std::cout << " ep = " << ((enPassantSquare == -1) ? "None" : pieceSquareNames[enPassantSquare]) << '\n';
	std::cout << " white kingside castle " << ((canWhiteKSCastle) ? "y" : "n") << '\n';
	std::cout << " white queenside castle " << ((canWhiteQSCastle) ? "y" : "n") << '\n';
	
	std::cout << " black kingside castle " << ((canBlackKSCastle) ? "y" : "n") << '\n';
	std::cout << " black queenside castle " << ((canBlackQSCastle) ? "y" : "n") << '\n';
	std::cout << "\n\n\n";
}

bool Board::movePawnFifthRank(const Square from, const Square to){	
	return ((pieces.getPiecesBB(Side::BLACK, PAWN) & RANK_7 & bitset(from)) && (RANK_5 & bitset(to)));
}

bool Board::movePawnFourthRank(const Square from, const Square to){

	return (pieces.getPiecesBB(Side::WHITE, PAWN) & RANK_2 & bitset(from)) && (RANK_4 & bitset(to));

}

void Board::addMoveToHistory(const Move& move){	
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
		Square target = static_cast<Square>((turn == Side::WHITE) ? to - 8 : to + 8);
	
		pieces.movePiece(turn, PAWN, to, from);
		pieces.addPiece(!turn, PAWN, target);
	}	
	else if (flag == W_KS_CASTLE_FLAG){

		pieces.movePiece(Side::WHITE, KING, G1, E1);
		pieces.movePiece(Side::WHITE, ROOK, F1, H1); 
	
	}
	else if (flag == B_KS_CASTLE_FLAG){
		
		pieces.movePiece(Side::BLACK, KING, G8, E8);
		pieces.movePiece(Side::BLACK, ROOK, F8, H8); 
		
	}	
 	else if (flag == W_QS_CASTLE_FLAG){

                pieces.movePiece(Side::WHITE, KING, C1, E1);
                pieces.movePiece(Side::WHITE, ROOK, D1, A1);
        	
	}
        else if (flag == B_QS_CASTLE_FLAG){

                pieces.movePiece(Side::BLACK, KING, C8, E8);
                pieces.movePiece(Side::BLACK, ROOK, D8, A8);
        	
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

	pieces.movePiece(Side::WHITE, KING, E1, G1);
	pieces.movePiece(Side::WHITE, ROOK, H1, F1); 
}

void Board::whiteQueenSideCastle(){

	pieces.movePiece(Side::WHITE, KING, E1, C1);
	pieces.movePiece(Side::WHITE, ROOK, A1, D1); 

}

void Board::blackKingSideCastle(){

	pieces.movePiece(Side::BLACK, KING, E8, G8);
	pieces.movePiece(Side::BLACK, ROOK, H8, F8); 

}

void Board::blackQueenSideCastle(){

	pieces.movePiece(Side::BLACK, KING, E8, C8);
	pieces.movePiece(Side::BLACK, ROOK, A8, D8); 
}


void Board::enPassantWhite(const Square from, const Square to){
		
		
			pieces.movePiece(Side::WHITE, PAWN, from, to);	
			
			
			Square square = static_cast<Square>(to-8);
			pieces.clearPiece(Side::BLACK, PAWN, square);



}

void Board::enPassantBlack(const Square from, const Square to){	
			pieces.movePiece(Side::BLACK, PAWN, from, to);

			Square square = static_cast<Square>(to+8);
            pieces.clearPiece(Side::WHITE, PAWN, square);

}


bool Board::isInCheck(Side side){
	Bitboard king_bb = pieces.getPiecesBB(side, KING);

	Square kingSquare = utils::pop_lsb(king_bb);
	return pieces.isAttacked(side, kingSquare);
}

int Board::getMaterialCount(Side side) const{

	return
	pieces.getPieceCount(side, KING) * KING_MATERIAL_VALUE + 	
	pieces.getPieceCount(side, QUEEN) * QUEEN_MATERIAL_VALUE + 
	pieces.getPieceCount(side, ROOK) * ROOK_MATERIAL_VALUE + 
	pieces.getPieceCount(side, BISHOP) * BISHOP_MATERIAL_VALUE + 
	pieces.getPieceCount(side, KNIGHT) * KNIGHT_MATERIAL_VALUE + 
	pieces.getPieceCount(side, PAWN) * PAWN_MATERIAL_VALUE; 
}
bool Board::isGameOver(){	
	return isCheckmated(Side::WHITE) || isCheckmated(Side::BLACK);
}

bool Board::isCheckmated(Side side){
	return !legalMovesCount && isInCheck(side); 

}

bool Board::isDraw(){
	return isStalemate() || isInsufficientMaterial();
}

bool Board::isStalemate(){
	return !legalMovesCount && (!isInCheck(turn) || !isInCheck(!turn));
}

bool Board::isInsufficientMaterial(){
	int whiteBishopCount = pieces.getPieceCount(Side::WHITE, BISHOP);
	int blackBishopCount = pieces.getPieceCount(Side::BLACK, BISHOP);
	int whiteKnightCount = pieces.getPieceCount(Side::WHITE, KNIGHT);
	int blackKnightCount = pieces.getPieceCount(Side::BLACK, KNIGHT);

	int whiteMaterialCount = getMaterialCount(Side::WHITE);
	int blackMaterialCount = getMaterialCount(Side::BLACK);

	return (
		( whiteMaterialCount == KING_MATERIAL_VALUE && blackMaterialCount == KING_MATERIAL_VALUE)
		|| ( whiteBishopCount == 1 && whiteMaterialCount == (KING_MATERIAL_VALUE + BISHOP_MATERIAL_VALUE) && blackMaterialCount == KING_MATERIAL_VALUE)
		|| ( blackBishopCount == 1 && blackMaterialCount == (KING_MATERIAL_VALUE + BISHOP_MATERIAL_VALUE) && whiteMaterialCount == KING_MATERIAL_VALUE)	
		|| ( whiteKnightCount == 1 && whiteMaterialCount == (KING_MATERIAL_VALUE + KNIGHT_MATERIAL_VALUE) && blackMaterialCount == KING_MATERIAL_VALUE)
		|| ( blackKnightCount == 1 && blackMaterialCount == (KING_MATERIAL_VALUE + KNIGHT_MATERIAL_VALUE) && whiteMaterialCount == KING_MATERIAL_VALUE)	
		|| ( whiteBishopCount == 1 && whiteMaterialCount == (KING_MATERIAL_VALUE + BISHOP_MATERIAL_VALUE) && blackBishopCount == 1 && blackMaterialCount == (KING_MATERIAL_VALUE + BISHOP_MATERIAL_VALUE))
		
	);
}
