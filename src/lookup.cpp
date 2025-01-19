
#pragma once

#include <iostream>
#include <random>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <array>

#include "types.h"
#include "utils.h"


#define TABLE_SIZE 4096


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


uint64_t generateWhiteBishopMask(int plain_square){
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
	
 uint64_t upLeftDirection = 0Ull;
 uint64_t downLeftDirection = 0Ull;
 uint64_t upRightDirection = 0Ull;
 uint64_t downRightDirection = 0Ull;

 int curRank = plain_square / 8;
 int curFile = plain_square % 8;

 int rank=0ULL, file=0ULL;

 // Up left
 for (rank = curRank+1, file = curFile-1; rank <= 7 && file >= 0; rank++, file--){ 
	 
	upLeftDirection |= (1ULL << (file + rank*8));
  
	if (occupancy & (1ULL << (file + rank*8))) break;
 }

 // Up right
 for (rank = curRank+1, file = curFile+1; rank <= 7 && file <= 7; rank++, file++){
 	 
	upRightDirection |= (1ULL << (file + rank*8));
	
	if (occupancy & (1ULL << (file + rank*8))) break;
	
 }
 

 for (rank = curRank-1, file = curFile+1; rank >=0 && file <= 7; rank--, file++){
	
	downLeftDirection |= (1ULL << (file + rank*8));
	
	if (occupancy & (1ULL << (file + rank*8))) break;
 } 

for (rank = curRank-1 , file = curFile-1; rank >= 0 && file >= 0; rank--, file--){
	downRightDirection |= (1ULL << (file + rank*8));
	if (occupancy & (1ULL << (file + rank*8))) break;	
} 

 return (upLeftDirection | downLeftDirection | upRightDirection | downRightDirection);
}


constexpr std::array<uint64_t, TABLE_SIZE> createBlockedBoards(uint64_t blocker_pieces, int MAX_PATTERNS){
	std::array<uint64_t, TABLE_SIZE> blockedBitboards{};


	for (int i = 0; i < TABLE_SIZE; i++){
		blockedBitboards[i] = 0;
	}

	int n = __builtin_popcountll(blocker_pieces);
       		
	for (int patternIndex = 0; patternIndex < (1 << n); patternIndex++){
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

uint64_t random_uint64() {

  uint64_t u1 = (uint64_t)(random()) & 0xFFFF; 
  uint64_t u2 = (uint64_t)(random()) & 0xFFFF;
  uint64_t u3 = (uint64_t)(random()) & 0xFFFF; 
  uint64_t u4 = (uint64_t)(random()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

uint64_t random_uint64_fewbits() {
  return random_uint64() & random_uint64() & random_uint64();
}

constexpr bool fillLookupTable(std::array<uint64_t, TABLE_SIZE>& lookupTable, int MAX_PATTERNS, uint64_t attackCombos, std::array<uint64_t,TABLE_SIZE> occupancyCombos, int square,  int type, uint64_t magicNumber){

		
		int i=0;
		for (i = 0; i < MAX_PATTERNS; i++){
                        int index = utils::generateMagicIndex(occupancyCombos[i], magicNumber, square, ROOK_MOVES); 
			uint64_t legalMoves=0ULL;
				
			switch(type) {
				case ROOK_MOVES:
					legalMoves = calcLegalRookMoves(square, occupancyCombos[i]);
					break;
				case BISHOP_MOVES:
					legalMoves = calcLegalBishopMoves(square, occupancyCombos[i]);
					break;
			}		
		

            
			if (lookupTable[index] == 0ULL){
                                lookupTable[index] = legalMoves;
			}
                        else if (lookupTable[index] != legalMoves){
				break;
                        }
                }

		
                
		if (i == MAX_PATTERNS ) {	
			return true;
        	}
		return false;
}


uint64_t generateMagicNumber(int square, uint64_t blockers, int type){ 
	
	int MAX_PATTERNS;
	uint64_t attackCombos;	

	switch(type){
		case ROOK_MOVES:
			MAX_PATTERNS = 1 << (64 - rookShifts[square]);    
			blockers = blockers & rookOccupancyMasks[square];
			attackCombos = generateWhiteRookMask(square);
			break;
		case BISHOP_MOVES:	
			MAX_PATTERNS = 1 << (64 - bishopShifts[square]);
			blockers = blockers & bishopOccupancyMasks[square];
			attackCombos = generateWhiteBishopMask(square);
			break;
	}

	std::array<uint64_t, TABLE_SIZE> occupancyCombos = createBlockedBoards(blockers, MAX_PATTERNS);

	std::array<uint64_t, TABLE_SIZE> lookupTable = {0ULL};
    	uint64_t magicNumber = 0;
     
       			
		while (true){ 
                magicNumber = random_uint64_fewbits();
                 
		if(__builtin_popcountll((magicNumber * attackCombos) & 0xFF00000000000000ULL) < 6) continue;
		
		std::fill(lookupTable.begin(), lookupTable.end(), 0);
		
		int i;
		for (i = 0; i < MAX_PATTERNS; i++){
                        int index = utils::generateMagicIndex(occupancyCombos[i], magicNumber, square, type);
			uint64_t legalMoves;
				
			switch(type) {
				case ROOK_MOVES:
					legalMoves = calcLegalRookMoves(square, occupancyCombos[i]);
					break;
				case BISHOP_MOVES:
					legalMoves = calcLegalBishopMoves(square, occupancyCombos[i]);
					break;
			}		
			
			if (lookupTable[index] == 0ULL){
                                lookupTable[index] = legalMoves;
				
			}
                        else if (lookupTable[index] != legalMoves){
				
				break;
                        }

                }
		if (i == MAX_PATTERNS ) {	
			break;
		}
	       }	

    return magicNumber;

}





