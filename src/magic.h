#include <iostream>
#include "lookup.h"
#include <array>


static const std::array<uint64_t, 64> bishopMagics = []{
	std::array<uint64_t, 64> result{};
	for(size_t square = 0; square < 64; square++){
		uint64_t magic = generateMagicNumber(square, bishopOccupancyMasks[square], 1);
		result[square] = magic;	
	}
	return result;
}();

static const std::array<uint64_t, 64> rookMagics = []{
	std::array<uint64_t, 64> result{};
	for(size_t square = 0; square < 64; square++){
		uint64_t magic = generateMagicNumber(square, rookOccupancyMasks[square], 0);
		result[square] = magic;	
	}
	return result;
}();

static const std::array<std::array<uint64_t, TABLE_SIZE>, 64> rookOccCombos = [] {

	std::array<std::array<uint64_t, TABLE_SIZE>, 64> result{};
	for(size_t square = 0; square < 64; square++){
		int MAX_PATTERNS = 1 << (64 - rookShifts[square]);   
		result[square] = createBlockedBoards(rookOccupancyMasks[square], MAX_PATTERNS);		
	}

	return result;

}();

static const std::array<std::array<uint64_t, TABLE_SIZE>, 64> bishopOccCombos = [] {

	std::array<std::array<uint64_t, TABLE_SIZE>, 64> result{};
	for(size_t square = 0; square < 64; square++){
		int MAX_PATTERNS = 1 << (64 - bishopShifts[square]);   
		result[square] = createBlockedBoards(bishopOccupancyMasks[square], MAX_PATTERNS);		
	}

	return result;

}();

static const std::array<std::array<uint64_t, 512>, 64> bishopMoveList = [] {
	
	std::array<std::array<uint64_t, 512>, 64> allBishopLookups{};
	
	for (size_t square = 0; square < 64; square++){
		int MAX_PATTERNS = 1 << (64 - bishopShifts[square]);    
		std::array<uint64_t, 512> bishopLookup {};	
		
		for (int i = 0; i < MAX_PATTERNS; i++){
			int index = utils::generateMagicIndex(bishopOccCombos[square][i], bishopMagics[square], square, 1);
			bishopLookup[index] = calcLegalBishopMoves(square, bishopOccCombos[square][i]);
		}
		allBishopLookups[square] = bishopLookup;
	}
	return allBishopLookups;
}();

static const std::array<std::array<uint64_t, TABLE_SIZE>, 64> rookMoveList = [] {
	
	std::array<std::array<uint64_t, TABLE_SIZE>, 64> allRookLookups{};
	
	for (size_t square = 0; square < 64; square++){
		int MAX_PATTERNS = 1 << (64 - rookShifts[square]);    
		uint64_t attackCombos = rookOccupancyMasks[square];
		std::array<uint64_t, TABLE_SIZE> rookLookup {};	
		
		for (int i = 0; i < MAX_PATTERNS; i++){
			int index = utils::generateMagicIndex(rookOccCombos[square][i], rookMagics[square], square, 0);
			rookLookup[index] = calcLegalRookMoves(square, rookOccCombos[square][i]);
		}
		allRookLookups[square] = rookLookup;
	}
	return allRookLookups;
}();

