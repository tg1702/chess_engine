#include <iostream>
#include "types.h"

#define bitset(nbit) ( 1ULL << (nbit))
//#define clear_lsb(x)	(x &= (x - 1ULL))
#define bitclear(value, index) (value &= ~(1ULL << index))
#pragma once

namespace utils{
	constexpr int pop_lsb(uint64_t& bb) {int x = __builtin_ctzll(bb); bb &= (bb - 1ULL); return x;}	
	constexpr int generateMagicIndex(uint64_t bitboard, uint64_t magicNumber, int square, int type){
        if (type == ROOK_MOVES)
                return (int)((bitboard * magicNumber) >> rookShifts[square]);
        else
                return (int)((bitboard * magicNumber) >> bishopShifts[square]);
}
	
}
