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
void score_moves(std::vector<Move>&);
void pick_move(std::vector<Move>&, size_t);
#endif