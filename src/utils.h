#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>
#include "types.h"


namespace utils{
	
	constexpr inline Square pop_lsb(Bitboard& bb) {
		int x = __builtin_ctzll(bb); 
		bb &= (bb - 1ULL); 
		return static_cast<Square>(x);
	}

	inline void clear_bb_at(Bitboard& value, Square sq) {
		value &= ~(1ULL << static_cast<int>(sq));
	}

	constexpr inline Bitboard to_bb(Square sq) {
		return 1ULL << static_cast<int>(sq);
	}

	constexpr inline Bitboard to_bb(int num) {
		return 1ULL << static_cast<int>(num);
	}
	
	inline  int generateMagicIndex(Bitboard bitboard, Bitboard magicNumber, int square, int type){
        if (type == ROOK_MOVES)
                return (int)((bitboard * magicNumber) >> rookShifts[square]);
        else
                return (int)((bitboard * magicNumber) >> bishopShifts[square]);
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