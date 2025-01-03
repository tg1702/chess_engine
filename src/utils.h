#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include "types.h"

#define bitset(nbit) ( 1ULL << (nbit))
#define bitclear(value, index) (value &= ~(1ULL << index))

namespace utils{
	
	static std::vector<std::string> split_string(std::string &str){
		std::stringstream ss(str);
		std::istream_iterator<std::string> begin(ss), end;
		return std::vector<std::string> (begin, end);
	}

	static constexpr Square pop_lsb(uint64_t& bb) {int x = __builtin_ctzll(bb); bb &= (bb - 1ULL); return static_cast<Square>(x);}
        static constexpr PieceType int_to_PieceType(int num){return static_cast<PieceType>(num);}	
	static constexpr int generateMagicIndex(uint64_t bitboard, uint64_t magicNumber, int square, int type){
        if (type == ROOK_MOVES)
                return (int)((bitboard * magicNumber) >> rookShifts[square]);
        else
                return (int)((bitboard * magicNumber) >> bishopShifts[square]);
	}

	
}
