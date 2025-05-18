
#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include "../types.h"
#include "../utils.h"


class Move{
	private:
		uint32_t value;
		int score;
	public:

		Move() = default;
		constexpr Move(Flag flag, Square from, Square to, PieceType fromPiece, PieceType toPiece=KING) : 
			value(((static_cast<int>(flag) & 0xf)<<18) | ((from & 0x3f)<<12) | ((to & 0x3f)<<6) | ((fromPiece & 0x7)<<3) | ((toPiece & 0x7))),
			score(0) {};
	
		
		inline Move(Move&& other) noexcept = default;
	
		Move& operator=(Move&& other) noexcept = default;

	
		inline Move(const Move& other) = default;

		Move& operator=(const Move& other) = default;
		
		constexpr Flag getFlag() const {return static_cast<Flag>((value >> 18) & 0xf);}
		constexpr Square getFrom() const {return static_cast<Square>((value >> 12) & 0x3f);}
		constexpr Square getTo() const {return static_cast<Square>((value >> 6) & 0x3f);}
		constexpr PieceType getFromPiece() const {return static_cast<PieceType>((value >> 3) & 0x7);}
		constexpr PieceType getToPiece() const {return static_cast<PieceType>((value) & 0x7);}
		
			
		inline void setTo(Square to) {value &= ~0xfc0; value |= (to & 0xfc0) << 6; }
		inline void setFrom(Square from) {value &= ~0x3f000; value |= (from & 0x3f000) << 12; }
		inline void setFlag(int flag) {value &= ~0xfc0000; value |= (flag & 0xfc0000) << 18;} 

		int get_score(){return this->score;}
		void set_score(int score){this->score = score;}

		friend std::ostream& operator<<(std::ostream& os, const Move& move){
			Flag flag = move.getFlag();
			std::string promoteCode = "";

			if (move.getTo() == H1 && move.getFrom() == H1)
				return os << "0000";
			
			if (flag == Flag::QUEEN_PROMOTION || flag == Flag::QUEEN_PROMOTION_CAPTURE)
				promoteCode = "q";

			if (flag == Flag::ROOK_PROMOTION || flag == Flag::ROOK_PROMOTION_CAPTURE)
				promoteCode = "r";

			if (flag == Flag::BISHOP_PROMOTION || flag == Flag::BISHOP_PROMOTION_CAPTURE)
				promoteCode = "b";

			if (flag == Flag::KNIGHT_PROMOTION || flag == Flag::KNIGHT_PROMOTION_CAPTURE)
				promoteCode = "n";

			return os << pieceSquareNames[move.getFrom()] << pieceSquareNames[move.getTo()] << promoteCode;

		}
};


struct MoveList{
	Move moves[MAX_MOVES];
	int count = 0;
};


#endif