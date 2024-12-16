#include <iostream>

#pragma once
class Move{
	private:
		uint32_t value;
	public:

		Move(){value = 0;} 
		Move(int flag, int from, int to, int fromPiece, int toPiece=0){
			value = ((flag & 0xf)<<18) | ((from & 0x3f)<<12) | ((to & 0x3f)<<6) | ((fromPiece & 0x7)<<3) | ((toPiece & 0x7));

		}
	
		void printMove() const {std::cout << "from = " << pieceSquareNames[getFrom()] << " to = " << pieceSquareNames[getTo()] << " flags = " << getFlag() << " fromPiece = " << getFromPiece() << " toPiece = " << getToPiece() << std::endl;}	
		uint32_t getFlag() const {return (value >> 18) & 0xf;}
		uint32_t getFrom() const {return (value >> 12) & 0x3f;}
		uint32_t getTo() const {return (value >> 6) & 0x3f;}
		uint32_t getFromPiece() const {return (value >> 3) & 0x7;}
		uint32_t getToPiece() const {return (value) & 0x7;}
		
		void operator=(Move a){value = a.value;}
		bool operator==(Move a){return value == a.value;}
		bool operator!=(Move a){return value != a.value;}

};
