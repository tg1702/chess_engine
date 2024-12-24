#include <iostream>

#pragma once
class Move{
	//private:
	public:

		uint32_t value;
		Move() = default;
		constexpr Move(int flag, int from, int to, int fromPiece, int toPiece=0) : 
			value(((flag & 0xf)<<18) | ((from & 0x3f)<<12) | ((to & 0x3f)<<6) | ((fromPiece & 0x7)<<3) | ((toPiece & 0x7))) {};
	
		
		inline Move(Move&& other) noexcept = default;
	
		Move& operator=(Move&& other) noexcept = default;

	
		inline Move(const Move& other) = default;

		Move& operator=(const Move& other) = default;

		//inline void printMove() const {std::cout << "from = " << pieceSquareNames[getFrom()] << " to = " << pieceSquareNames[getTo()] << " flags = " << getFlag() << " fromPiece = " << getFromPiece() << " toPiece = " << getToPiece() << std::endl;}	
		constexpr uint32_t getFlag() const {return (value >> 18) & 0xf;}
		constexpr uint32_t getFrom() const {return (value >> 12) & 0x3f;}
		constexpr uint32_t getTo() const {return (value >> 6) & 0x3f;}
		constexpr uint32_t getFromPiece() const {return (value >> 3) & 0x7;}
		constexpr uint32_t getToPiece() const {return (value) & 0x7;}
	
		inline void setTo(int to) {value &= ~0xfc0; value |= (to & 0xfc0) << 6; }
		inline void setFrom(int from) {value &= ~0x3f000; value |= (from & 0x3f000) << 12; }
		inline void setFlag(int flag) {value &= ~0xfc0000; value |= (flag & 0xfc0000) << 18;} 
		//inline void operator=(Move &a){value = a.value;}
		//inline bool operator==(Move &a){return value == a.value;}
		//inline bool operator!=(Move &a){return value != a.value;}

};
