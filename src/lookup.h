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




void writeToTextFile(array<uint64_t, TABLE_SIZE>);
uint64_t generateWhiteRookMask(int);
uint64_t generateWhiteBishopMask(int);
uint64_t calcLegalBishopMoves(int, uint64_t);
int generateMagicIndex(uint64_t, uint64_t, int, int);
uint64_t* createBlockedBoards(uint64_t, int);
uint64_t random_uint64(void);
uint64_t generateMagicNumber(int, uint64_t, int);

