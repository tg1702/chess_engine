#include "types.h"
#include "utils.h"

namespace utils{
uint64_t getRankMask(uint64_t pos){
	uint64_t rank_location = 0;
	for (int i = 0; i < 8; ++i){
		if (ranks[i] & pos)
			rank_location = rank_location | ranks[i];
	}
	return rank_location;
}


int count_ones(unsigned long long bb){
  int count = 0;
  while (bb){
    if (bb & 1)
      count++;

    bb >>= 1;

  }
  return count;
}

int generateMagicIndex(uint64_t bitboard, uint64_t magicNumber, int square, int type){
        if (type == ROOK_MOVES)
                return (int)((bitboard * magicNumber) >> rookShifts[square]);
        else
                return (int)((bitboard * magicNumber) >> bishopShifts[square]);
}

}
