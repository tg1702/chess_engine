#include <iostream>
#include <random>
#include <algorithm>
#include "types.h"
#include <set>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#pragma once


uint64_t outputRank(uint64_t);
uint64_t generateWhitePawnMoves(uint64_t);
uint64_t generateWhiteKingMoves(uint64_t);
uint64_t generateWhiteKnightMoves(uint64_t);
uint64_t generateWhiteRookMask(uint64_t);


void writeToTextFile(array<uint64_t, 4096> lookupTable){
	ofstream file;
	file.open("magic.txt", std::ios_base::app);
	file << endl;
	file << "{ ";
	for (int i = 0; i < lookupTable.size(); ++i){
          	
		file << lookupTable[i] << "ULL";
		if (i != lookupTable.size() - 1)	
			file << ", ";
	}

	file << "},";
	file.close();

}
uint64_t generateWhiteRookMask(int plain_square){
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

 leftDirection &= outputRank(square);
 rightDirection &= outputRank(square);

 return upDirection | downDirection | leftDirection | rightDirection;
}

int generateMagicIndex(uint64_t bitboard, uint64_t magicNumber, int square){
	return  (int)((bitboard * magicNumber) >> rookShifts[square]);
}

int countr_zero(uint64_t bb){
	int count = 0;
	while ((bb & 1) == 0){
		count += 1;
		bb >>= 1;
	}
	return count;	
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

uint64_t* createBlockedBoards(uint64_t blocked_pieces, int MAX_PATTERNS){
	vector<int> blockedIndices;
	static uint64_t blockedBitboards[4096] = {0ULL};
	for (int index = 0; index < 64; index++){
		if (((blocked_pieces >> index) & 1 )){
	       	blockedIndices.push_back(index);
		}
	}

	for (int i = 0; i < 4096; i++){
		blockedBitboards[i] = 0;
	}
       		
	for (int patternIndex = 0; patternIndex < MAX_PATTERNS; patternIndex++){
		for (size_t bitIndex = 0; bitIndex < blockedIndices.size(); bitIndex++){
			int bit = (patternIndex >> bitIndex) & 1;
			blockedBitboards[patternIndex] |= (uint64_t)bit << blockedIndices[bitIndex];
		}
	}	
	return blockedBitboards;	
}

uint64_t calcLegalRookMoves(int plain_square, uint64_t occupancy){
	uint64_t legalMoves = 0ULL;
	
	
	
 uint64_t square = bitset(plain_square);
 uint64_t upDirection = 0Ull;
 uint64_t downDirection = 0Ull;
 uint64_t leftDirection = 0Ull;
 uint64_t rightDirection = 0Ull;
 for (int i = 8; i <= 56; i+= 8){
	 upDirection |= square << i;

	if (occupancy & (square << i)) break;
 }

 for (int i = 8; i <= 56; i+=8){
	
	 downDirection |= square >> i;
	if (occupancy & (square >> i)) break;
 }

 for (int i = 1; i <= 7; i++){
	leftDirection |= (square << i);

	if (occupancy & (square << i)) break;
 }


 for (int i = 1; i <=7; i++){
	
       	rightDirection |= (square >> i);

	if ((occupancy & (square >> i) )) break;
 }

 leftDirection &= outputRank(square);
 rightDirection &= outputRank(square);

 return upDirection | downDirection | leftDirection | rightDirection;
}

uint64_t random_uint64() {
  uint64_t u1, u2, u3, u4;
  u1 = (uint64_t)(random()) & 0xFFFF; u2 = (uint64_t)(random()) & 0xFFFF;
  u3 = (uint64_t)(random()) & 0xFFFF; u4 = (uint64_t)(random()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

uint64_t random_uint64_fewbits() {
  return random_uint64() & random_uint64() & random_uint64();
}

uint64_t generateMagicNumber(int square, uint64_t blockers){ 
	
	int MAX_PATTERNS = 1 << (64 - rookShifts[square]);    
	blockers = blockers & rookOccupancyMasks[square];
	uint64_t* occupancyCombos = createBlockedBoards(blockers, MAX_PATTERNS);
        uint64_t attackCombos = generateWhiteRookMask(square);

    	array<uint64_t, 4096> lookupTable = {0ULL};
    	uint64_t magicNumber = 0;
     
       			
		while (true){ 
                magicNumber = random_uint64_fewbits();
                 
		if(count_ones((magicNumber * attackCombos) & 0xFF00000000000000ULL) < 6) continue;
		
		fill(lookupTable.begin(), lookupTable.end(), 0);
		
		int i;
		for (i = 0; i < MAX_PATTERNS; i++){
                        int index = generateMagicIndex(occupancyCombos[i], magicNumber, square);
			uint64_t legalMoves = calcLegalRookMoves(square, occupancyCombos[i]);	
			
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

	writeToTextFile(lookupTable);	

        return magicNumber;

}


uint64_t outputRank(uint64_t pos){
	uint64_t rank_location = 0;
	for (int i = 0; i < 8; ++i){
		if (ranks[i] & pos)
			rank_location = rank_location | ranks[i];
	}
	return rank_location;
}

