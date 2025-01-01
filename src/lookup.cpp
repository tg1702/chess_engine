#include <iostream>
#include "lookup.h"
#include "magic.h"
#include "types.h"

int main(){
	/*
	std::cout << "constexpr std::array<uint64_t, 64> rookMagics = {\n";
	for(size_t square = 0; square < 64; square++){

		uint64_t magic = generateMagicNumber(square, rookOccupancyMasks[square], ROOK_MOVES);
		std::cout << magic << "ULL," << '\n';
	}

	std::cout << "}\n";
	*/

	for(size_t square = 0; square < 64; square++){

		int index = utils::generateMagicIndex(bishopOccupancyMasks[square], bishopMagics[square], square, 1);

		std::array<std::array<uint64_t, TABLE_SIZE>, 64> result{};
	
		int MAX_PATTERNS = 1 << (64 - rookShifts[square]);   
		result[square] = createBlockedBoards(bishopOccupancyMasks[square], MAX_PATTERNS);		

	
	std::cout << result[square][index]<< '\n';
    std::cout << rookMoveList[square][index] << '\n';
    std::cout << "------------------------\n";
  }


	
	//std::cout << rookMoveList[1] << '\n';
	
	return 0;
}


