#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>
#include "types.h"

#define bitset(nbit) ( 1ULL << (nbit))
#define bitclear(value, index) (value &= ~(1ULL << index))

namespace utils{
	
	inline constexpr Square pop_lsb(Bitboard& bb) {
		int x = __builtin_ctzll(bb); 
		bb &= (bb - 1ULL); 
		return static_cast<Square>(x);
	}

	inline constexpr PieceType int_to_PieceType(int num){return static_cast<PieceType>(num);}	
	
	inline constexpr int generateMagicIndex(Bitboard bitboard, Bitboard magicNumber, int square, int type){
        if (type == ROOK_MOVES)
                return (int)((bitboard * magicNumber) >> rookShifts[square]);
        else
                return (int)((bitboard * magicNumber) >> bishopShifts[square]);
	}

	inline constexpr Side flip_side(const Side side) { 
		bool s = static_cast<bool>(side);
		s = !s;

		return static_cast<Side>(s);

	}
	std::vector<std::string> split_string(std::string);

	template<typename T>
	int findPos(std::vector<T>& vec, T item) {
		auto iterator = std::find(vec.begin(), vec.end(), item);
		if ( iterator != vec.end())
			return iterator - vec.begin();
		
		return -1;
	}

	
}

#endif