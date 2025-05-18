#ifndef SEARCH_H
#define SEARCH_H

#include <iostream>
#include <atomic>
#include "../board.h"
#include "../movegen/move.h"
#include <vector>


extern std::atomic<bool> isSearching;

float negamax(int, Board&, float, float, int);
float evaluate(Board&);
void search(Board&, int, int, int);
float quiescence_search(Board&, int, int, float, float);
void score_moves(Moves&);
void pick_move(Moves&, size_t);
#endif