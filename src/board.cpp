#include "board.h"
#include "move.h"
#include "movegen.h"
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
		
			 
			enPassantWhiteLeftFromSquare = 0;
                        enPassantBlackLeftFromSquare = 0;

                        enPassantWhiteRightFromSquare = 0;
                        enPassantBlackRightFromSquare = 0;

                        enPassantWhiteToSquare = 0;
                        enPassantBlackToSquare = 0;			

			enPassantWhiteFlag = false;
			enPassantBlackFlag = false;
			enPassantSquare = -1;
			//addCastlingRights();
			
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
	
			enPassantWhiteLeftFromSquare = 0;
			enPassantBlackLeftFromSquare = 0;
			
			enPassantWhiteRightFromSquare = 0;
			enPassantBlackRightFromSquare = 0;
		
			enPassantWhiteToSquare = 0;
			enPassantBlackToSquare = 0;	
			enPassantWhiteFlag = false;
			enPassantBlackFlag = false;
			enPassantSquare = -1;	
			//addCastlingRights();

}
void Board::addCastlingRights(){
	
			if (turn == WHITE){
			
				if (pieces.canKingSideCastle(WHITE) && !hasMovedH1Rook && !hasMovedWhiteKing)
					moves.addKingSideCastling(WHITE);

				if (pieces.canQueenSideCastle(WHITE) && !hasMovedA1Rook && !hasMovedWhiteKing)
					moves.addQueenSideCastling(WHITE);
			}


			else {
				if (pieces.canKingSideCastle(BLACK) && !hasMovedH8Rook && !hasMovedBlackKing)
                                	moves.addKingSideCastling(BLACK);
                       
				if (pieces.canQueenSideCastle(BLACK) && !hasMovedA8Rook && !hasMovedBlackKing)
                                	moves.addQueenSideCastling(BLACK);			
			}
}

std::string Board::makeMove(Move& m){
	int from = m.getFrom();
	int to = m.getTo();
	int special = m.getFlag();

	int pieceType = m.getFromPiece();
	bool validCapture = false;
	size_t toPieceType = 0;
	size_t capturedPieceType = 0;

	std::string moveMessage = "Invalid move " + pieceSquareNames[from] + pieceSquareNames[to];	


	//cout << " en passant white flag " << enPassantWhiteFlag << endl;	
		//for (size_t pieceType = 0; pieceType < PIECE_TYPES; pieceType++){
			//if (isValidMove(pieceType, from, to)){
			if (pieceType == KING && isWhiteKSCastlingMove(from, to))
			{

				special = W_KS_CASTLE_FLAG;	
				whiteKingSideCastle();	
			}

			else if (pieceType == KING && isWhiteQSCastlingMove(from, to)){
			
				special = W_QS_CASTLE_FLAG;
				whiteQueenSideCastle();	
			}

			else if (pieceType == KING && isBlackKSCastlingMove(from, to)){

				special = B_KS_CASTLE_FLAG;
				blackKingSideCastle();
			}

			else if (pieceType == KING && isBlackQSCastlingMove(from, to)){

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
				special = EN_PASSANT_FLAG;
				enPassantWhite(from, to);
			}


			else if ( pieceType == PAWN && turn == BLACK && special == EN_PASSANT_FLAG){
				special = EN_PASSANT_FLAG;	
				enPassantBlack(from, to);
			}


			else {
				special = NORMAL;	

				if (turn == BLACK && pieceType == PAWN && movePawnFifthRank(from, to)){
					//cout << "fifth rank check " << endl;
				
					//cout << " left of to " << pieceSquareNames[(to - 1)] << " right of to " << pieceSquareNames[(to + 1)] << '\n' ;	
					//std::cout << "black double pawn push " << endl;	
					enPassantSquare = to;
					
					//if (enPassantWhiteLeftFromSquare)
						//cout << "ep black moved " << pieceSquareNames[from] << pieceSquareNames[to] << "ep from to " << pieceSquareNames[enPassantWhiteLeftFromSquare] << pieceSquareNames[enPassantWhiteToSquare] << '\n';

					//if (enPassantWhiteRightFromSquare)
						//cout << "ep black moved " << pieceSquareNames[from] << pieceSquareNames[to] << "ep from to " << pieceSquareNames[enPassantWhiteRightFromSquare] << pieceSquareNames[enPassantWhiteToSquare] << '\n';
				}

				else if (turn == WHITE && pieceType == PAWN && movePawnFourthRank(from, to) ){
					enPassantSquare = to;
					//std::cout << " to = " << pieceSquareNames[to] << " next to " << pieceSquareNames[to-1] << " and " << pieceSquareNames[to+1] << '\n';
				}
				
					
				pieces.movePiece(turn, pieceType, from, to);
				validCapture = true;


				if (pieceType == KING && turn == WHITE) hasMovedWhiteKing = true;
				if (pieceType == KING && turn == BLACK) hasMovedBlackKing = true;
				if (pieceType == ROOK && (pieces.getPiecesBB(turn, ROOK) & bitset(A1))	&& turn == WHITE) hasMovedA1Rook = true;
				if (pieceType == ROOK && (pieces.getPiecesBB(turn, ROOK) & bitset(H1))	&& turn == WHITE) hasMovedH1Rook = true;
				if (pieceType == ROOK && (pieces.getPiecesBB(turn, ROOK) & bitset(A8))	&& turn == BLACK) hasMovedA8Rook = true;
				if (pieceType == ROOK && (pieces.getPiecesBB(turn, ROOK) & bitset(H8))	&& turn == BLACK) hasMovedH8Rook = true;
		
			
			}
			
                         moveMessage = pieceSquareNames[from] + pieceSquareNames[to];
			 toPieceType = pieceType;
		//}
		

	//}
		
	
	for (size_t i = 0; i < PIECE_TYPES && validCapture; i++){
		if ((pieces.getPiecesBB(!turn, i) & bitset(to))){ 
			special = updateToCaptureFlag(special);
				
			pieces.clearPiece(!turn, i, to);
			capturedPieceType = i;
		} 

	}

		
	//cout << "captures: " << captures << endl;
	//if (from == D5 && to == E6)
		//cout << "special " << endl;
	addMoveToHistory(Move(special, from, to, toPieceType, capturedPieceType));	

	pieces.setSidePiecesBB(turn);
	pieces.setSidePiecesBB(!turn);


	//std::cout<< " white pieces all " << pieces.getPiecesBB(WHITE, ALL) << '\n';
	//std::cout << "black pieces all " << pieces.getPiecesBB(BLACK, ALL) << '\n';
	
	/*
	if (from == G1 && to == F3){
	cout << "Move " << pieceSquareNames[from] << pieceSquareNames[to] << endl;	
	cout << "pieces for side " << pieces.getPiecesBB(turn, ALL) << endl;
	cout << "pieces for opponent " << pieces.getPiecesBB(!turn, ALL) << endl;	
	
	}
*/	
	turn = !turn;	
	
	
	
	return moveMessage;		
}

MoveList Board::generateMoves(){

	moves.clearMoves();

	PieceBB friendly {.king_bb = pieces.getPiecesBB(turn, KING), .queen_bb = pieces.getPiecesBB(turn, QUEEN), .rook_bb = pieces.getPiecesBB(turn, ROOK),  .bishop_bb = pieces.getPiecesBB(turn, BISHOP), .knight_bb = pieces.getPiecesBB(turn, KNIGHT), .pawn_bb = pieces.getPiecesBB(turn, PAWN)};
		
	moves.generateMoves(turn, &friendly, pieces.getPiecesBB(!turn, ALL));
	

	addEnPassantRights();	
	enPassantSquare = -1;
	
	return getMovesList();
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
		std::cout << endl;
	}
	std::cout << "\n\n\n";
}

bool Board::movePawnFifthRank(int from, int to){	
	return ((pieces.getPiecesBB(BLACK, PAWN) & RANK_7 & bitset(from)) && (RANK_5 & bitset(to)));
}

bool Board::movePawnFourthRank(int from, int to){

	return (pieces.getPiecesBB(WHITE, PAWN) & RANK_2 & bitset(from)) && (RANK_4 & bitset(to));

}

void Board::addMoveToHistory(Move move){
	actualMoves.push(move);
}

void Board::printHistory(){
	std::stack temp = actualMoves;

	while (!temp.empty())
	{
		Move lastMove = temp.top();
		temp.pop();
		lastMove.printMove();
	}
	std::cout << "----------------------" <<'\n';
}

int Board::getActualMoveCount(){
	return actualMoves.size();
}
void Board::setActualMoveCount(int depth){
	//actualMoveCount = depth;
}
std::string Board::unmakeMove(){
	std::string undoMessage = "Invalid undo!";
	turn = !turn;

	Move lastMove = actualMoves.top();
	
	actualMoves.pop();	
	int piece = lastMove.getFromPiece();
	int from = lastMove.getFrom();
	int to = lastMove.getTo();
	int capturedPieceType = lastMove.getToPiece();
	int flag = lastMove.getFlag();	
	
//	if (from == E5 && to == D6 && piece == PAWN)
		//std::cout << "flag = " << flag << '\n';

	/*
	
if (actualMoves[0].getFrom() == G1 && actualMoves[0].getTo() == H3){
		printHistory();
	}
*/
	if (flag == NORMAL){	
		pieces.movePiece(turn, piece, to, from); 
/*	
		if (from == G1 && to == H3){	
		cout << "in undo bb" << pieces.getPiecesBB(turn, piece) << endl;
		cout << "in undo attacks bb" << pieces.getMovesBB(turn, piece) << endl;
		}*/
		
		undoMessage = "Undoing normal move " + pieceSquareNames[from] + pieceSquareNames[to];
	}
	else if (flag == CAPTURE_FLAG){
	
			
		pieces.addPiece(!turn, capturedPieceType, to);
		pieces.movePiece(turn, piece, to, from);

		undoMessage = "Undoing capture move " + pieceSquareNames[from] + pieceSquareNames[to];
	}
	else if (flag == EN_PASSANT_FLAG){
		int target = 0;

		(turn == WHITE) ? target = to - 8 : target = to + 8;
	
		captures += 1;
		//std::cout << "ep captures " << captures << '\n';		
		
		pieces.movePiece(turn, PAWN, to, from);
		pieces.addPiece(!turn, PAWN, target);
		undoMessage = "Undoing en passant capture " + pieceSquareNames[from] + pieceSquareNames[to];	
	}	
	else if (flag == W_KS_CASTLE_FLAG){

		pieces.movePiece(WHITE, KING, G1, E1);
		pieces.movePiece(WHITE, ROOK, F1, H1); 
		undoMessage =  "Undoing white kingside castle "+ pieceSquareNames[from] + pieceSquareNames[to];
	}
	else if (flag == B_KS_CASTLE_FLAG){
		
		pieces.movePiece(BLACK, KING, G8, E8);
		pieces.movePiece(BLACK, ROOK, F8, H8); 
		
		undoMessage = "Undoing black kingside castle " + pieceSquareNames[from] + pieceSquareNames[to];
	}	
 	else if (flag == W_QS_CASTLE_FLAG){

                pieces.movePiece(WHITE, KING, C1, E1);
                pieces.movePiece(WHITE, ROOK, D1, A1);
        	
		undoMessage = "Undoing white queenside castle " + pieceSquareNames[from] + pieceSquareNames[to];
	}
        else if (flag == B_QS_CASTLE_FLAG){

                pieces.movePiece(BLACK, KING, C8, E8);
                pieces.movePiece(BLACK, ROOK, D8, A8);
        	
		undoMessage = "Undoing black queenside castle " + pieceSquareNames[from] + pieceSquareNames[to];
	}
	else if (flag == QUEEN_PROMOTION){
		pieces.clearPiece(turn, QUEEN, to);
		pieces.addPiece(turn, PAWN, from);	
		undoMessage = "Undoing queen promotion " + pieceSquareNames[from] + pieceSquareNames[to];
	}
	else if (flag == ROOK_PROMOTION){
                pieces.clearPiece(turn, ROOK, to);
                pieces.addPiece(turn, PAWN, from);
                undoMessage = "Undoing rook promotion " + pieceSquareNames[from] + pieceSquareNames[to];
        }
	else if (flag == BISHOP_PROMOTION){
                pieces.clearPiece(turn, BISHOP, to);
                pieces.addPiece(turn, PAWN, from);
                undoMessage = "Undoing bishop promotion " + pieceSquareNames[from] + pieceSquareNames[to];
        }
	else if (flag == KNIGHT_PROMOTION){
                pieces.clearPiece(turn, KNIGHT, to);
                pieces.addPiece(turn, PAWN, from);
                undoMessage = "Undoing knight promotion " + pieceSquareNames[from] + pieceSquareNames[to];
        }
	else if (flag == QUEEN_PROMOTION_CAPTURE){
                pieces.clearPiece(turn, QUEEN, to);
                pieces.addPiece(turn, PAWN, from);
		pieces.addPiece(!turn, capturedPieceType, to);
                undoMessage = "Undoing queen promotion capture " + pieceSquareNames[from] + pieceSquareNames[to];
        }
        else if (flag == ROOK_PROMOTION_CAPTURE){
                pieces.clearPiece(turn, ROOK, to);
                pieces.addPiece(turn, PAWN, from);
                pieces.addPiece(!turn, capturedPieceType, to);
		undoMessage = "Undoing rook promotion capture " + pieceSquareNames[from] + pieceSquareNames[to];
        }
        else if (flag == BISHOP_PROMOTION_CAPTURE){
                pieces.clearPiece(turn, BISHOP, to);
                pieces.addPiece(turn, PAWN, from);
                pieces.addPiece(!turn, capturedPieceType, to);
		undoMessage = "Undoing bishop promotion capture " + pieceSquareNames[from] + pieceSquareNames[to];
        }
        else if (flag == KNIGHT_PROMOTION_CAPTURE){
                pieces.clearPiece(turn, KNIGHT, to);
                pieces.addPiece(turn, PAWN, from);
                pieces.addPiece(!turn, capturedPieceType, to);
		undoMessage = "Undoing knight promotion capture " + pieceSquareNames[from] + pieceSquareNames[to];
        }	
	
	if (undoMessage.find("Invalid") != std::string::npos)
		return undoMessage;

	pieces.setSidePiecesBB(turn);
	pieces.setSidePiecesBB(!turn);
	
	
	PieceBB friendly {.king_bb = pieces.getPiecesBB(turn, KING), .queen_bb = pieces.getPiecesBB(turn, QUEEN), .rook_bb = pieces.getPiecesBB(turn, ROOK),  .bishop_bb = pieces.getPiecesBB(turn, BISHOP), .knight_bb = pieces.getPiecesBB(turn, KNIGHT), .pawn_bb = pieces.getPiecesBB(turn, PAWN)};
	
	moves.generateMoves(turn, &friendly, pieces.getPiecesBB(!turn, ALL));
	
	addEnPassantRights();
 	
	enPassantSquare = -1;
	return undoMessage;	
}

void Board::addEnPassantRights(){
	if (enPassantSquare == -1) return;

	uint64_t rank = (turn == WHITE) ? RANK_5 : RANK_4;
	
	//std::cout << rank << '\n';	
//	std::cout << pieces.getPiecesBB(turn, PAWN) << '\n';

	//std::cout << "adj squares " << (bitset(enPassantSquare - 1) | bitset(enPassantSquare + 1)) << '\n';
	//std::cout << "piece " << (pieces.getPiecesBB(turn, PAWN)) << '\n';
	//std::cout << "rank " << rank << '\n';
	//std::cout << "------------------**" << '\n';
	uint64_t candidates = pieces.getPiecesBB(turn, PAWN) & rank & ( bitset(enPassantSquare - 1) | bitset(enPassantSquare + 1)) ;

	
	while (candidates != 0ULL){	
		int from = __builtin_ctzll(candidates);

		
		if (turn == WHITE)
			moves.addEnPassantRights(WHITE, from , enPassantSquare+8); 
		

		else if (turn == BLACK)	
			moves.addEnPassantRights(BLACK,  from , enPassantSquare-8); 
		
		bitclear(candidates, from);
	
}

}
bool Board::isValidMove(int pieceType, int from, int to){
	//if (from == G1 && to == F3)
		//cout << "Nf3 attacks " << pieces.getMovesBB(turn, pieceType) << endl;
	return ((pieces.getPiecesBB(turn, pieceType) & bitset(from)) && (pieces.getMovesBB(turn, pieceType) & bitset(to)));
}

bool Board::isCastlingMove(int from, int to){
	return isWhiteKSCastlingMove(from, to) || isWhiteQSCastlingMove(from, to) || isBlackKSCastlingMove(from, to) || isBlackQSCastlingMove(from, to);  
}

bool Board::isWhiteKSCastlingMove(int from, int to){
	return (turn == WHITE && from == E1 && to == G1); 
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
/*
void Board::removeEnPassantRights(){

	if (turn == BLACK && enPassantWhiteFlag){	
	
		if (enPassantWhiteLeftFromSquare)
                        moves.removeEnPassantRights(WHITE, enPassantWhiteLeftFromSquare, enPassantWhiteToSquare);
                if (enPassantWhiteRightFromSquare)
                        moves.removeEnPassantRights(WHITE, enPassantWhiteRightFromSquare, enPassantWhiteToSquare);
	
		
		enPassantWhiteFlag = false;
 		enPassantWhiteLeftFromSquare = 0;

                enPassantWhiteRightFromSquare = 0;

                enPassantWhiteToSquare = 0;
	}
	else if (turn == WHITE && enPassantBlackFlag) {
	
	 	if (enPassantBlackLeftFromSquare)
                        moves.removeEnPassantRights(BLACK, enPassantBlackLeftFromSquare, enPassantBlackToSquare);
                if (enPassantBlackRightFromSquare)
                        moves.removeEnPassantRights(BLACK, enPassantBlackRightFromSquare, enPassantBlackToSquare);		
		
		enPassantBlackFlag = false;
		enPassantBlackLeftFromSquare = 0;
		enPassantBlackRightFromSquare = 0;		
		enPassantBlackToSquare = 0;
		
	}

}
*/

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
			//cout << "ep ing " << from << to << endl;
		
	//		std::cout << "trying to ep as white" << pieceSquareNames[from] << pieceSquareNames[to] << '\n';	
		
			pieces.movePiece(WHITE, PAWN, from, to);	
			pieces.clearPiece(BLACK, PAWN, to-8);



			//cout << pieces.getPiecesBB(WHITE, ALL) << endl;
			//cout << pieces.getPiecesBB(BLACK, ALL) << endl;
			
			//cout << "-------------------------------" << endl;
}

void Board::enPassantBlack(int from, int to){	
			//std::cout << "trying to ep " << pieceSquareNames[from] << pieceSquareNames[to] << '\n';	
			pieces.movePiece(BLACK, PAWN, from, to);
                        pieces.clearPiece(WHITE, PAWN, to+8);

}

bool Board::isWhiteEnPassantMove(int to){
	return turn == WHITE && (enPassantSquare != -1);
}

bool Board::isBlackEnPassantMove(int to){
	return turn == BLACK && (enPassantSquare != -1);
}

bool Board::isInCheck(bool side){
	int kingSquare = utils::countr_zero(pieces.getPiecesBB(side, KING));
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
