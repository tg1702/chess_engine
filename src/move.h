
#pragma once

#include <iostream>
#include "types.h"
#include "utils.h"

class Move{
	private:
		uint32_t value;
	public:

		Move() = default;
		constexpr Move(int flag, Square from, Square to, PieceType fromPiece, PieceType toPiece=utils::int_to_PieceType(0)) : 
			value(((flag & 0xf)<<18) | ((from & 0x3f)<<12) | ((to & 0x3f)<<6) | ((fromPiece & 0x7)<<3) | ((toPiece & 0x7))) {};
	
		
		inline Move(Move&& other) noexcept = default;
	
		Move& operator=(Move&& other) noexcept = default;

	
		inline Move(const Move& other) = default;

		Move& operator=(const Move& other) = default;
		
		constexpr uint32_t getFlag() const {return (value >> 18) & 0xf;}
		constexpr Square getFrom() const {return static_cast<Square>((value >> 12) & 0x3f);}
		constexpr Square getTo() const {return static_cast<Square>((value >> 6) & 0x3f);}
		constexpr PieceType getFromPiece() const {return static_cast<PieceType>((value >> 3) & 0x7);}
		constexpr PieceType getToPiece() const {return static_cast<PieceType>((value) & 0x7);}
	
		inline void setTo(Square to) {value &= ~0xfc0; value |= (to & 0xfc0) << 6; }
		inline void setFrom(Square from) {value &= ~0x3f000; value |= (from & 0x3f000) << 12; }
		inline void setFlag(int flag) {value &= ~0xfc0000; value |= (flag & 0xfc0000) << 18;} 

};


struct MoveList{
	Move moves[MAX_MOVES];
	int count = 0;
};
