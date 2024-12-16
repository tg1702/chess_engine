#include "board.h"
#include "move.h"
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

			//	pieces.generateAllMoves(turn);	
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
	
			enPassantWhiteLeftFromSquare = 0;
			enPassantBlackLeftFromSquare = 0;
			
			enPassantWhiteRightFromSquare = 0;
			enPassantBlackRightFromSquare = 0;
		
			enPassantWhiteToSquare = 0;
			enPassantBlackToSquare = 0;	
			enPassantWhiteFlag = false;
			enPassantBlackFlag = false;
			
//			pieces.generateAllMoves(turn);
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

std::string Board::makeMove(int from, int to, int special){
	bool validCapture = false;
	size_t toPieceType = 0;
	size_t capturedPieceType = 0;

	std::string moveMessage = "Invalid move " + pieceSquareNames[from] + pieceSquareNames[to];	


	//cout << " en passant white flag " << enPassantWhiteFlag << endl;	
		for (size_t pieceType = 0; pieceType < PIECE_TYPES; pieceType++){
		if (isValidMove(pieceType, from, to)){
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

			else if ( pieceType == PAWN && isWhiteEnPassantMove(to)){
				special = EN_PASSANT_FLAG;
				enPassantWhite(from, to);
			}


			else if ( pieceType == PAWN && isBlackEnPassantMove(to)){
				special = EN_PASSANT_FLAG;	
				enPassantBlack(from, to);	
			}


			else {
				special = NORMAL;	

		
				if (turn == BLACK && pieceType == PAWN && movePawnFifthRank(from, to)){
					//cout << "fifth rank check " << endl;
					enPassantWhiteFlag = true;
					
					enPassantWhiteToSquare = to + 8;
				
					//cout << " left of to " << pieceSquareNames[(to - 1)] << " right of to " << pieceSquareNames[(to + 1)] << '\n' ;	
					if ( bitset(to)<<1 & pieces.getPiecesBB(WHITE, PAWN) & utils::getRankMask(bitset(to)))
						enPassantWhiteLeftFromSquare = to+1;
					if ( bitset(to)>>1 & pieces.getPiecesBB(WHITE, PAWN) & utils::getRankMask(bitset(to)) )	
						enPassantWhiteRightFromSquare = to-1;
					
					//if (enPassantWhiteLeftFromSquare)
						//cout << "ep black moved " << pieceSquareNames[from] << pieceSquareNames[to] << "ep from to " << pieceSquareNames[enPassantWhiteLeftFromSquare] << pieceSquareNames[enPassantWhiteToSquare] << '\n';

					//if (enPassantWhiteRightFromSquare)
						//cout << "ep black moved " << pieceSquareNames[from] << pieceSquareNames[to] << "ep from to " << pieceSquareNames[enPassantWhiteRightFromSquare] << pieceSquareNames[enPassantWhiteToSquare] << '\n';
				}

				else if (turn == WHITE && pieceType == PAWN && movePawnFourthRank(from, to) ){
					enPassantBlackFlag = true;
					 if ( bitset(to+1) & pieces.getPiecesBB(BLACK, PAWN) & utils::getRankMask(bitset(to)))
                                                enPassantBlackLeftFromSquare = to+1;
                                         if ( bitset(to-1) & pieces.getPiecesBB(BLACK, PAWN) & utils::getRankMask(bitset(to)))
                                                enPassantBlackRightFromSquare = to-1;
					
					enPassantBlackToSquare = to  - 8;
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
		}
	

	}
		
	
	for (size_t i = 0; i < PIECE_TYPES && validCapture; i++){
		if (pieces.getPiecesBB(!turn, i) & bitset(to)){ 
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

std::vector<Move> Board::generateMoves(){
	
	pieces.clearMoves();	
	
	pieces.generateAllMoves(turn);
	
	addEnPassantRights();	
	removeEnPassantRights();	
	setMoves();

	return getMoves();
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
	return ((pieces.getPiecesBB(BLACK, PAWN) & RANK_7 & bitset(from)) && (pieces.getMovesBB(BLACK, PAWN) & RANK_5 & bitset(to)));
}

bool Board::movePawnFourthRank(int from, int to){

	return (( pieces.getPiecesBB(BLACK, PAWN) & (bitset(to+1) | bitset(to-1)) & utils::getRankMask(to) & RANK_4) && pieces.getPiecesBB(WHITE, PAWN) & RANK_2 & bitset(from)) && (pieces.getMovesBB(WHITE, PAWN) & RANK_4 & bitset(to));

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

		turn ? target = to - 8 : target = to + 8;
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
	
	pieces.generateAllMoves(turn);	
	addEnPassantRights();

	return undoMessage;	
}

void Board::addEnPassantRights(){
	if (turn == WHITE && enPassantWhiteFlag) {
		//cout << "white flag set " << endl;
		//cout << enPassantWhiteLeftFromSquare << endl;
		//cout << enPassantWhiteRightFromSquare << endl;	
		if (enPassantWhiteLeftFromSquare)
			pieces.addEnPassantRights(WHITE, enPassantWhiteLeftFromSquare, enPassantWhiteToSquare); 
		if (enPassantWhiteRightFromSquare)
			pieces.addEnPassantRights(WHITE, enPassantWhiteRightFromSquare, enPassantWhiteToSquare); 
	}

	if (turn == BLACK && enPassantBlackFlag){	
		if (enPassantBlackLeftFromSquare)
			pieces.addEnPassantRights(BLACK, enPassantBlackLeftFromSquare, enPassantBlackToSquare);
		if (enPassantBlackRightFromSquare)
			pieces.addEnPassantRights(BLACK, enPassantBlackRightFromSquare, enPassantBlackToSquare); 
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

void Board::removeEnPassantRights(){

	if (turn == BLACK && enPassantWhiteFlag){	
		if (enPassantWhiteLeftFromSquare)
                        pieces.removeEnPassantRights(WHITE, enPassantWhiteLeftFromSquare, enPassantWhiteToSquare);
                if (enPassantWhiteRightFromSquare)
                        pieces.removeEnPassantRights(WHITE, enPassantWhiteRightFromSquare, enPassantWhiteToSquare);
		
		enPassantWhiteFlag = false;
 		enPassantWhiteLeftFromSquare = 0;

                enPassantWhiteRightFromSquare = 0;

                enPassantWhiteToSquare = 0;
	}
	else if (turn == WHITE && enPassantBlackFlag) {
		
	 	if (enPassantBlackLeftFromSquare)
                        pieces.removeEnPassantRights(BLACK, enPassantBlackLeftFromSquare, enPassantBlackToSquare);
                if (enPassantBlackRightFromSquare)
                        pieces.removeEnPassantRights(BLACK, enPassantBlackRightFromSquare, enPassantBlackToSquare);		
		enPassantBlackFlag = false;
		enPassantBlackLeftFromSquare = 0;
		enPassantBlackRightFromSquare = 0;		
		enPassantBlackToSquare = 0;
		
	}

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
			//cout << "ep ing " << from << to << endl;
			uint64_t blackPawnBB = pieces.getPiecesBB(BLACK, PAWN);
		
			pieces.movePiece(WHITE, PAWN, from, to);	
			pieces.clearPiece(BLACK, PAWN, enPassantWhiteToSquare-8);

			//cout << pieces.getPiecesBB(WHITE, ALL) << endl;
			//cout << pieces.getPiecesBB(BLACK, ALL) << endl;
			
			enPassantWhiteFlag = false;
 			enPassantWhiteLeftFromSquare = 0;

                	enPassantWhiteRightFromSquare = 0;

                	enPassantWhiteToSquare = 0;
}

void Board::enPassantBlack(int from, int to){

			uint64_t whitePawnBB = pieces.getPiecesBB(WHITE, PAWN);
		
			
			pieces.movePiece(BLACK, PAWN, from, to);
                        pieces.clearPiece(WHITE, PAWN, enPassantBlackToSquare+8);

			enPassantBlackFlag = false;
			enPassantBlackLeftFromSquare = 0;
			enPassantWhiteRightFromSquare = 0;		
			enPassantBlackToSquare = 0;
}

bool Board::isWhiteEnPassantMove(int to){
	return turn == WHITE && enPassantWhiteFlag && (bitset(to) & bitset(enPassantWhiteToSquare));
}

bool Board::isBlackEnPassantMove(int to){
	return turn == BLACK && (bitset(to) & bitset(enPassantBlackToSquare));
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
