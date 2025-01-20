#include <iostream>
#include <atomic>
#include "board.h"
#include "move.h"

#pragma once
extern std::atomic<bool> isSearching;

float negamax(int, Board&, float, float, int);
float evaluate(Board&);
void search(Board&, int, int, int);

