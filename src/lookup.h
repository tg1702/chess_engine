#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <array>

#include "types.h"
#include "utils.h"
// Max size of lookup table (redefine as 512 for bishops)
#define TABLE_SIZE 4096
using namespace std;

#pragma once

static void writeToTextFile(std::array<uint64_t, TABLE_SIZE> lookupTable){
	ofstream file;
	file.open("magic.txt", std::ios_base::app);
	file << " {\n";
	for (int i = 0; i < lookupTable.size(); ++i){
          	
		file << lookupTable[i] << "ULL";
		if (i != lookupTable.size() - 1)	
			file << ", ";
	}

	file << "},\n";
	file.close();

}
constexpr uint64_t generateWhiteRookMask(int plain_square){
 uint64_t square = bitset(plain_square);
 uint64_t upDirection = 0Ull;
 uint64_t downDirection = 0Ull;
 uint64_t leftDirection = 0Ull;
 uint64_t rightDirection = 0Ull;
 for (int i = 8; i <= 56; i+= 8){
	 upDirection |= square << i;
	 downDirection |= square >> i;
 }

 for (int i = 1; i <= 7; i++){
	leftDirection |= square << i;
       	rightDirection |= square >> i;	
 }


 //leftDirection &= bitset(curRank);
 //rightDirection &= bitset(curRank);

 return upDirection | downDirection | leftDirection | rightDirection;
}


static uint64_t generateWhiteBishopMask(int plain_square){
 uint64_t square = bitset(plain_square);
 uint64_t upLeftDirection = 0Ull;
 uint64_t downLeftDirection = 0Ull;
 uint64_t upRightDirection = 0Ull;
 uint64_t downRightDirection = 0Ull;


 for (int i = 7; i <= 49; i+= 7){ 
	 
	 if (((square) & FILE_H)) break;
	 upRightDirection |= square << i;
 
	 if (((square << i) & RANK_1) || ((square << i) & RANK_8) || ((square << i) & FILE_A) || ((square << i) & FILE_H)) break;
 }

 for (int i = 9; i <= 63; i+= 9){
	 if (square & FILE_H) break; 
 	 downRightDirection |= square >> i;
	
	 if (((square >> i) & RANK_1) || ((square >> i) & RANK_8) || ((square >> i) & FILE_A) || ((square >> i) & FILE_H)) break;
 }
 
 
 for (int i = 9; i <= 63; i+= 9){
	
	if (((square) & FILE_A)) break;
	upLeftDirection |= square << i;
	
	if (((square << i) & RANK_1) || ((square << i) & RANK_8) || ((square << i) & FILE_A) || ((square << i) & FILE_H)) break;
 } 

for (int i = 7; i <= 49; i+= 7){
        if (square & FILE_A) break; 
	downLeftDirection |= square >> i;
	if (((square >> i) & RANK_1) || ((square >> i) & RANK_8) || ((square >> i) & FILE_A) || ((square >> i) & FILE_H)) break;
} 

 return upLeftDirection | downLeftDirection | upRightDirection | downRightDirection;
}


constexpr uint64_t calcLegalBishopMoves(int plain_square, uint64_t occupancy){
	
 uint64_t square = bitset(plain_square);
 uint64_t upLeftDirection = 0Ull;
 uint64_t downLeftDirection = 0Ull;
 uint64_t upRightDirection = 0Ull;
 uint64_t downRightDirection = 0Ull;

 int curRank = plain_square / 8;
 int curFile = plain_square % 8;

 for (int rank = curRank+1; rank <= 7; rank++){ 
	 
	upRightDirection |= (1ULL << (curFile + rank*8));
  
	if (occupancy & (1ULL << (curFile + rank*8))) break;
 }

 for (int rank = curRank-1; rank >= 0; rank--){
 	 
	downRightDirection |= (1ULL << (curFile + rank*8));
	
	if (occupancy & (1ULL << (curFile + rank*8))) break;
	
 }
 

 for (int file = curFile+1; file <= 7; file++){
	
	upLeftDirection |= (1ULL << (file + curRank*8));
	
	if (occupancy & (1ULL << (file + curRank*8))) break;
 } 

for (int file = curFile-1; file >= 0; file--){
	downLeftDirection |= (1ULL << (file + curRank*8));

	if (occupancy & (1ULL << (file + curRank*8))) break;	
} 

 return (upLeftDirection | downLeftDirection | upRightDirection | downRightDirection);
}


constexpr std::array<uint64_t, TABLE_SIZE> createBlockedBoards(uint64_t blocker_pieces, int MAX_PATTERNS){
	int count = 0;
	std::array<uint64_t, TABLE_SIZE> blockedBitboards{};


	for (int i = 0; i < TABLE_SIZE; i++){
		blockedBitboards[i] = 0;
	}

	int n = __builtin_popcountll(blocker_pieces);
       		
	for (int patternIndex = 0; patternIndex < (1ULL << n); patternIndex++){
		uint64_t result = 0ULL;
		uint64_t copy = blocker_pieces;

		for (int bitIndex = 0; bitIndex < n; bitIndex++){
			int index = utils::pop_lsb(copy);
			if (patternIndex & (1 << bitIndex)) 	result |= (1ULL << index);
		}
		blockedBitboards[patternIndex] = result;
	}
		
	return blockedBitboards;	
}

constexpr uint64_t calcLegalRookMoves(int plain_square, uint64_t occupancy){
	uint64_t legalMoves = 0ULL;
	
	
	
 uint64_t result = 0ULL;
 
 int curRank = plain_square / 8;
 int curFile = plain_square % 8;
 
 //std::cout << " current rank" << curRank << '\n';
 for (int file = curFile+1; file <= 7; file++){
	result |= (1ULL << (file + curRank*8));

	if (occupancy & (1ULL << (file + curRank*8))) break;
 }

 for (int file = curFile-1; file >= 0; file--){
	
	result |= (1ULL << (file + curRank*8));
	if (occupancy & (1ULL << (file + curRank*8))) break;
 }

 for (int rank = curRank+1; rank <= 7; rank++){
	result |= (1ULL << (curFile + rank*8));

	if (occupancy & (1ULL << (curFile + rank*8))) break;
 }


 for (int rank = curRank-1; rank >= 0; rank--){
	
       	result |= (1ULL << (curFile + rank*8));

	if ((occupancy & (1ULL << (curFile + rank*8)) )) break;
 }

 return result;
}

static uint64_t random_uint64() {

  uint64_t u1 = (uint64_t)(random()) & 0xFFFF; 
  uint64_t u2 = (uint64_t)(random()) & 0xFFFF;
  uint64_t u3 = (uint64_t)(random()) & 0xFFFF; 
  uint64_t u4 = (uint64_t)(random()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

static uint64_t random_uint64_fewbits() {
  return random_uint64() & random_uint64() & random_uint64();
}

constexpr bool fillLookupTable(std::array<uint64_t, TABLE_SIZE>& lookupTable, int MAX_PATTERNS, uint64_t attackCombos, std::array<uint64_t,TABLE_SIZE> occupancyCombos, int square,  int type, uint64_t magicNumber){

		
		int i=0;
		for (i = 0; i < MAX_PATTERNS; i++){
                        int index = utils::generateMagicIndex(occupancyCombos[i], magicNumber, square, 0);
                        lookupTable[index] = calcLegalRookMoves(square, occupancyCombos[i]);
            /*            
			uint64_t legalMoves=0ULL;
				
			switch(type) {
				case ROOK_MOVES:
					legalMoves = calcLegalRookMoves(square, occupancyCombos[i]);
					break;
				case BISHOP_MOVES:
					legalMoves = calcLegalBishopMoves(square, occupancyCombos[i]);
					break;
			}		
			*/

            /*
			if (lookupTable[index] == 0ULL){
                                lookupTable[index] = legalMoves;
				//std::cout << legalMoves << '\n';	
			}
                        else if (lookupTable[index] != legalMoves){
				
				break;
                        }
*/
                }

		
                
		if (i == MAX_PATTERNS ) {	
			return true;
        }
		return false;
}

static uint64_t generateMagicNumber(int square, uint64_t blockers, int type){ 
	
	std::array<uint64_t, TABLE_SIZE> lookupTable{};

	int MAX_PATTERNS=0;
	uint64_t attackCombos=0ULL;	

	switch(type){
		case ROOK_MOVES:
			MAX_PATTERNS = 1 << (64 - rookShifts[square]);    
			blockers = blockers & rookOccupancyMasks[square];
			attackCombos = rookOccupancyMasks[square];
			break;
		case BISHOP_MOVES:	
			MAX_PATTERNS = 1 << (64 - bishopShifts[square]);
			blockers = blockers & bishopOccupancyMasks[square];
			attackCombos = bishopOccupancyMasks[square];
			break;
	}
		
	std::array<uint64_t, TABLE_SIZE> occupancyCombos = createBlockedBoards(blockers, MAX_PATTERNS);
	

    	uint64_t magicNumber = 0;
     
       			
	while (true){ 
                magicNumber = random_uint64_fewbits();
                
		if(__builtin_popcountll((magicNumber * calcLegalRookMoves(square, occupancyCombos[square])) & 0xFF00000000000000ULL) < 6) continue;
	
		std::fill(lookupTable.begin(), lookupTable.end(), 0);
		
		bool filled = fillLookupTable(lookupTable, MAX_PATTERNS, attackCombos, occupancyCombos, square, type, magicNumber);	
		if (filled) break;
	}


        return magicNumber;

}


