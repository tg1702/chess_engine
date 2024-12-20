#include <iostream>
#include "types.h"

#define bitset(nbit) ( 1ULL << nbit)
#define clear_lsb(x)	(x &= (x - 1ULL))
#define bitclear(value, index) (value &= ~(1ULL << index))
#pragma once

namespace utils{
	uint64_t getRankMask(uint64_t);
	inline int countr_zero(uint64_t bb) {return __builtin_ctzll(bb);}	
	int count_ones(unsigned long long);
	int generateMagicIndex(unsigned long, unsigned long, int , int);

	
}
