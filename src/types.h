#include <iostream>
#include <string>
#include <array>
#include <unordered_map>
#include "move.h"

#ifndef TYPES_H
#define TYPES_H


constexpr int MAX_MOVES = 256;

enum SlidingPieces{
        ROOK_MOVES,
        BISHOP_MOVES
};

enum sides{
	WHITE,
	BLACK
};

enum PieceType{
	KING,
	ROOK,
	PAWN,
	BISHOP,
	QUEEN,
	KNIGHT,
	ALL
};

const std::array<PieceType, 6> PieceTypes= {
	KING,
	ROOK,
	PAWN,
	BISHOP,
	QUEEN,
	KNIGHT
};

struct PieceBB {
	uint64_t king_bb = 0ULL;
	uint64_t queen_bb = 0ULL;
	uint64_t rook_bb = 0ULL;

	uint64_t bishop_bb = 0ULL;
	uint64_t knight_bb = 0ULL;
	uint64_t pawn_bb = 0ULL;

};

enum Square{
H1, G1, F1, E1, D1, C1, B1, A1,

H2, G2, F2, E2, D2, C2, B2, A2,

H3, G3, F3, E3, D3, C3, B3, A3,

H4, G4, F4, E4, D4, C4, B4, A4,

H5, G5, F5, E5, D5, C5, B5, A5,

H6, G6, F6, E6, D6, C6, B6, A6,

H7, G7, F7, E7, D7, C7, B7, A7,

H8, G8, F8, E8, D8, C8, B8, A8
};

const std::array<Square, 64> Squares = {
H1, G1, F1, E1, D1, C1, B1, A1,

H2, G2, F2, E2, D2, C2, B2, A2,

H3, G3, F3, E3, D3, C3, B3, A3,

H4, G4, F4, E4, D4, C4, B4, A4,

H5, G5, F5, E5, D5, C5, B5, A5,

H6, G6, F6, E6, D6, C6, B6, A6,

H7, G7, F7, E7, D7, C7, B7, A7,

H8, G8, F8, E8, D8, C8, B8, A8
};

const std::array<std::string, 64> pieceSquareNames = {
"h1", "g1", "f1", "e1", "d1", "c1", "b1", "a1",

"h2", "g2", "f2", "e2", "d2", "c2", "b2", "a2",

"h3", "g3", "f3", "e3", "d3", "c3", "b3", "a3",

"h4", "g4", "f4", "e4", "d4", "c4", "b4", "a4",

"h5", "g5", "f5", "e5", "d5", "c5", "b5", "a5",

"h6", "g6", "f6", "e6", "d6", "c6", "b6", "a6",

"h7", "g7", "f7", "e7", "d7", "c7", "b7", "a7",

"h8", "g8", "f8", "e8", "d8", "c8", "b8", "a8"
};	

const std::unordered_map<std::string, int> pieceSquareValues ={
	{"h1", H1},
	{"g1", G1},
	{"f1", F1},
	{"e1", E1},
	{"d1", D1},
	{"c1", C1},
	{"b1", B1},
	{"a1", A1},



	{"h2", H2},
	{"g2", G2},
	{"f2", F2},
	{"e2", E2},
	{"d2", D2},
	{"c2", C2},
	{"b2", B2},
	{"a2", A2},


	{"h3", H3},
	{"g3", G3},
	{"f3", F3},
	{"e3", E3},
	{"d3", D3},
	{"c3", C3},
	{"b3", B3},
	{"a3", A3},



	{"h4", H4},
	{"g4", G4},
	{"f4", F4},
	{"e4", E4},
	{"d4", D4},
	{"c4", C4},
	{"b4", B4},
	{"a4", A4},

	{"h5", H5},
	{"g5", G5},
	{"f5", F5},
	{"e5", E5},
	{"d5", D5},
	{"c5", C5},
	{"b5", B5},
	{"a5", A5},
	
	{"h6", H6},
	{"g6", G6},
	{"f6", F6},
	{"e6", E6},
	{"d6", D6},
	{"c6", C6},
	{"b6", B6},
	{"a6", A6},

	{"h7", H7},
	{"g7", G7},
	{"f7", F7},
	{"e7", E7},
	{"d7", D7},
	{"c7", C7},
	{"b7", B7},
	{"a7", A7},

	{"h8", H8},
	{"g8", G8},
	{"f8", F8},
	{"e8", E8},
	{"d8", D8},
	{"c8", C8},
	{"b8", B8},
	{"a8", A8},

};
enum flags{
	NORMAL,
	QUEEN_PROMOTION,
	ROOK_PROMOTION,
	BISHOP_PROMOTION,
	KNIGHT_PROMOTION,
	QUEEN_PROMOTION_CAPTURE,
        ROOK_PROMOTION_CAPTURE,
        BISHOP_PROMOTION_CAPTURE,
        KNIGHT_PROMOTION_CAPTURE,
	W_KS_CASTLE_FLAG,
	B_KS_CASTLE_FLAG,
	W_QS_CASTLE_FLAG,
	B_QS_CASTLE_FLAG,
	EN_PASSANT_FLAG,
	CAPTURE_FLAG
};

constexpr int normal_promoted_codes[4] = {

	QUEEN_PROMOTION,
	ROOK_PROMOTION,
	BISHOP_PROMOTION,
	KNIGHT_PROMOTION

};

constexpr int capture_promoted_codes[4] = {

	QUEEN_PROMOTION_CAPTURE,
        ROOK_PROMOTION_CAPTURE,
        BISHOP_PROMOTION_CAPTURE,
        KNIGHT_PROMOTION_CAPTURE,

};
struct PieceArgs{
	
		uint64_t w_king_bb = 0;
		uint64_t w_rooks_bb = 0;
		uint64_t w_pawns_bb = 0;
		uint64_t w_bishops_bb = 0;
		uint64_t w_queens_bb = 0;
		uint64_t w_knights_bb = 0;
		
		uint64_t b_king_bb = 0; 
		uint64_t b_pawns_bb = 0;
		uint64_t b_queens_bb = 0;
		uint64_t b_bishops_bb = 0;
		uint64_t b_rooks_bb = 0;
		uint64_t b_knights_bb = 0;	
};


enum StartingPositions{
	W_KING_START = 0b0000000000000000000000000000000000000000000000000000000000001000,
	W_ROOKS_START = 0b0000000000000000000000000000000000000000000000000000000010000001,
	W_PAWNS_START = 0b0000000000000000000000000000000000000000000000001111111100000000,
        W_BISHOPS_START = 0b0000000000000000000000000000000000000000000000000000000000100100,
	W_QUEEN_START = 0b0000000000000000000000000000000000000000000000000000000000010000,
	W_KNIGHTS_START = 0b0000000000000000000000000000000000000000000000000000000001000010,

	B_KING_START = 0b0000100000000000000000000000000000000000000000000000000000000000,
	B_PAWNS_START = 0b0000000011111111000000000000000000000000000000000000000000000000,
	B_QUEEN_START = 0b0001000000000000000000000000000000000000000000000000000000000000,
	B_BISHOPS_START = 0b0010010000000000000000000000000000000000000000000000000000000000,
	B_ROOKS_START = 0b1000000100000000000000000000000000000000000000000000000000000000,
	B_KNIGHTS_START = 0b0100001000000000000000000000000000000000000000000000000000000000,

	B_ALL_PIECES_START = 0b1111111111111111000000000000000000000000000000000000000000000000,
	W_ALL_PIECES_START = 0b0000000000000000000000000000000000000000000000001111111111111111,

	RANK_1 = 0x00000000000000FF,
	RANK_2 = 0x000000000000FF00,
	RANK_3 = 0x0000000000FF0000,
	RANK_4 = 0x00000000FF000000,
	RANK_5 = 0x000000FF00000000,
	RANK_6 = 0x0000FF0000000000,
	RANK_7 = 0x00FF000000000000,
	RANK_8 = 0xFF00000000000000,

	FILE_A = 0x8080808080808080,
	FILE_B = 0x4040404040404040,
	FILE_C = 0x2020202020202020,
	FILE_D = 0x1010101010101010,
	FILE_E = 0x0808080808080808,
	FILE_F = 0x0404040404040404,
	FILE_G = 0x0202020202020202,
	FILE_H = 0x0101010101010101
};

constexpr uint64_t ranks[] = {RANK_1, RANK_2, RANK_3, RANK_4,RANK_5, RANK_6, RANK_7, RANK_8};


constexpr uint64_t kingLookups[64] = {770ULL,1797ULL,3594ULL,7188ULL,14376ULL,28752ULL,57504ULL,49216ULL,197123ULL,460039ULL,920078ULL,1840156ULL,3680312ULL,7360624ULL,14721248ULL,12599488ULL,50463488ULL,117769984ULL,235539968ULL,471079936ULL,942159872ULL,1884319744ULL,3768639488ULL,3225468928ULL,12918652928ULL,30149115904ULL,60298231808ULL,120596463616ULL,241192927232ULL,482385854464ULL,964771708928ULL,825720045568ULL,3307175149568ULL,7718173671424ULL,15436347342848ULL,30872694685696ULL,61745389371392ULL,123490778742784ULL,246981557485568ULL,211384331665408ULL,846636838289408ULL,1975852459884544ULL,3951704919769088ULL,7903409839538176ULL,15806819679076352ULL,31613639358152704ULL,63227278716305408ULL,54114388906344448ULL,216739030602088448ULL,505818229730443264ULL,1011636459460886528ULL,2023272918921773056ULL,4046545837843546112ULL,8093091675687092224ULL,16186183351374184448ULL,13853283560024178688ULL,144959613005987840ULL,362258295026614272ULL,724516590053228544ULL,1449033180106457088ULL,2898066360212914176ULL,5796132720425828352ULL,11592265440851656704ULL,4665729213955833856ULL};

constexpr uint64_t knightLookups[64] = {
	0x20400ULL,
0x50800ULL,
0xa1100ULL,
0x142200ULL,
0x284400ULL,
0x508800ULL,
0xa01000ULL,
0x402000ULL,
0x2040004ULL,
0x5080008ULL,
0xa110011ULL,
0x14220022ULL,
0x28440044ULL,
0x50880088ULL,
0xa0100010ULL,
0x40200020ULL,
0x204000402ULL,
0x508000805ULL,
0xa1100110aULL,
0x1422002214ULL,
0x2844004428ULL,
0x5088008850ULL,
0xa0100010a0ULL,
0x4020002040ULL,
0x20400040200ULL,
0x50800080500ULL,
0xa1100110a00ULL,
0x142200221400ULL,
0x284400442800ULL,
0x508800885000ULL,
0xa0100010a000ULL,
0x402000204000ULL,
0x2040004020000ULL,
0x5080008050000ULL,
0xa1100110a0000ULL,
0x14220022140000ULL,
0x28440044280000ULL,
0x50880088500000ULL,
0xa0100010a00000ULL,
0x40200020400000ULL,
0x204000402000000ULL,
0x508000805000000ULL,
0xa1100110a000000ULL,
0x1422002214000000ULL,
0x2844004428000000ULL,
0x5088008850000000ULL,
0xa0100010a0000000ULL,
0x4020002040000000ULL,
0x400040200000000ULL,
0x800080500000000ULL,
0x1100110a00000000ULL,
0x2200221400000000ULL,
0x4400442800000000ULL,
0x8800885000000000ULL,
0x100010a000000000ULL,
0x2000204000000000ULL,
0x4020000000000ULL,
0x8050000000000ULL,
0x110a0000000000ULL,
0x22140000000000ULL,
0x44280000000000ULL,
0x88500000000000ULL,
0x10a00000000000ULL,
0x20400000000000ULL
};

constexpr uint64_t pawnAttackLookups[2][64] = {
{ 512ULL, 1280ULL, 2560ULL, 5120ULL, 10240ULL, 20480ULL, 40960ULL, 16384ULL, 131072ULL, 327680ULL, 655360ULL, 1310720ULL, 2621440ULL, 5242880ULL, 10485760ULL, 4194304ULL, 33554432ULL, 83886080ULL, 167772160ULL, 335544320ULL, 671088640ULL, 1342177280ULL, 2684354560ULL, 1073741824ULL, 8589934592ULL, 21474836480ULL, 42949672960ULL, 85899345920ULL, 171798691840ULL, 343597383680ULL, 687194767360ULL, 274877906944ULL, 2199023255552ULL, 5497558138880ULL, 10995116277760ULL, 21990232555520ULL, 43980465111040ULL, 87960930222080ULL, 175921860444160ULL, 70368744177664ULL, 562949953421312ULL, 1407374883553280ULL, 2814749767106560ULL, 5629499534213120ULL, 11258999068426240ULL, 22517998136852480ULL, 45035996273704960ULL, 18014398509481984ULL, 144115188075855872ULL, 360287970189639680ULL, 720575940379279360ULL, 1441151880758558720ULL, 2882303761517117440ULL, 5764607523034234880ULL, 11529215046068469760ULL, 4611686018427387904ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL}
,

{ 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 2ULL, 5ULL, 10ULL, 20ULL, 40ULL, 80ULL, 160ULL, 64ULL, 512ULL, 1280ULL, 2560ULL, 5120ULL, 10240ULL, 20480ULL, 40960ULL, 16384ULL, 131072ULL, 327680ULL, 655360ULL, 1310720ULL, 2621440ULL, 5242880ULL, 10485760ULL, 4194304ULL, 33554432ULL, 83886080ULL, 167772160ULL, 335544320ULL, 671088640ULL, 1342177280ULL, 2684354560ULL, 1073741824ULL, 8589934592ULL, 21474836480ULL, 42949672960ULL, 85899345920ULL, 171798691840ULL, 343597383680ULL, 687194767360ULL, 274877906944ULL, 2199023255552ULL, 5497558138880ULL, 10995116277760ULL, 21990232555520ULL, 43980465111040ULL, 87960930222080ULL, 175921860444160ULL, 70368744177664ULL, 562949953421312ULL, 1407374883553280ULL, 2814749767106560ULL, 5629499534213120ULL, 11258999068426240ULL, 22517998136852480ULL, 45035996273704960ULL, 18014398509481984ULL}
};
constexpr uint64_t pawnLookups[2][64] = {
	{
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0x10000ULL,
	0x20000ULL,
	0x40000ULL,
	0x80000ULL,
	0x100000ULL,
	0x200000ULL,
	0x400000ULL,
	0x800000ULL,

0x1000000ULL,
0x2000000ULL,
0x4000000ULL,
0x8000000ULL,
0x10000000ULL,
0x20000000ULL,
0x40000000ULL,
0x80000000ULL,
0x100000000ULL,
0x200000000ULL,
0x400000000ULL,
0x800000000ULL,
0x1000000000ULL,
0x2000000000ULL,
0x4000000000ULL,
0x8000000000ULL,
0x10000000000ULL,
0x20000000000ULL,
0x40000000000ULL,
0x80000000000ULL,
0x100000000000ULL,
0x200000000000ULL,
0x400000000000ULL,
0x800000000000ULL,
0x1000000000000ULL,
0x2000000000000ULL,
0x4000000000000ULL,
0x8000000000000ULL,
0x10000000000000ULL,
0x20000000000000ULL,
0x40000000000000ULL,
0x80000000000000ULL,
0x100000000000000ULL,
0x200000000000000ULL,
0x400000000000000ULL,
0x800000000000000ULL,
0x1000000000000000ULL,
0x2000000000000000ULL,
0x4000000000000000ULL,
0x8000000000000000ULL,

0,
0,
0,
0,
0,
0,
0,
0
	},
{
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	
	0x1ULL,
	0x2ULL,
	0x4ULL,
	0x8ULL,
	0x10ULL,
	0x20ULL,
	0x40ULL,
	0x80ULL,
	
	
	0x100ULL,
	0x200ULL,
	0x400ULL,
	0x800ULL,
	0x1000ULL,
	0x2000ULL,
	0x4000ULL,
	0x8000ULL,

	0x10000ULL,
	0x20000ULL,
	0x40000ULL,
	0x80000ULL,
	0x100000ULL,
	0x200000ULL,
	0x400000ULL,
	0x800000ULL,


	0x1000000ULL,
	0x2000000ULL,
	0x4000000ULL,
	0x8000000ULL,
	0x10000000ULL,
	0x20000000ULL,
	0x40000000ULL,
	0x80000000ULL,
		
			
	0x100000000ULL,
	0x200000000ULL,
	0x400000000ULL,
	0x800000000ULL,
	0x1000000000ULL,
	0x2000000000ULL,
	0x4000000000ULL,
	0x8000000000ULL,

        0x10000000000ULL,
	0x20000000000ULL,
	0x40000000000ULL,
	0x80000000000ULL,
	0x100000000000ULL,
	0x200000000000ULL,
	0x400000000000ULL,
	0x800000000000ULL,

	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL	
}
};




constexpr uint64_t rookOccupancyMasks[64] = {
    0x000101010101017EULL, 0x000202020202027CULL, 0x000404040404047AULL,  0x8080808080876ULL,
    0x001010101010106EULL, 0x002020202020205EULL, 0x004040404040403EULL, 0x008080808080807EULL,
    0x0001010101017E00ULL, 0x0002020202027C00ULL, 0x0004040404047A00ULL, 0x0008080808087600ULL,
    0x0010101010106E00ULL, 0x0020202020205E00ULL, 0x0040404040403E00ULL, 0x0080808080807E00ULL,
    0x00010101017E0100ULL, 0x00020202027C0200ULL, 0x00040404047A0400ULL, 0x0008080808760800ULL,
    0x00101010106E1000ULL, 0x00202020205E2000ULL, 0x00404040403E4000ULL, 0x00808080807E8000ULL,
    0x000101017E010100ULL, 0x000202027C020200ULL, 0x000404047A040400ULL, 0x0008080876080800ULL,
    0x001010106E101000ULL, 0x002020205E202000ULL, 0x004040403E404000ULL, 0x008080807E808000ULL,
    0x0001017E01010100ULL, 0x0002027C02020200ULL, 0x0004047A04040400ULL, 0x0008087608080800ULL,
    0x0010106E10101000ULL, 0x0020205E20202000ULL, 0x0040403E40404000ULL, 0x0080807E80808000ULL,
    0x00017E0101010100ULL, 0x00027C0202020200ULL, 0x00047A0404040400ULL, 0x0008760808080800ULL,
    0x00106E1010101000ULL, 0x00205E2020202000ULL, 0x00403E4040404000ULL, 0x00807E8080808000ULL,
    0x007E010101010100ULL, 0x007C020202020200ULL, 0x007A040404040400ULL, 0x0076080808080800ULL,
    0x006E101010101000ULL, 0x005E202020202000ULL, 0x003E404040404000ULL, 0x007E808080808000ULL,
	0x7E01010101010100ULL, 0x7C02020202020200ULL, 0x7A04040404040400ULL, 0x7608080808080800ULL,
	0x6E10101010101000ULL, 0x5E20202020202000ULL, 0x3E40404040404000ULL, 0x7E80808080808000ULL
};

constexpr uint64_t bishopOccupancyMasks[64] = {
  18049651735527936ULL,
70506452091904ULL,
275415828992ULL,
1075975168ULL,
38021120ULL,
8657588224ULL,
2216338399232ULL,
567382630219776ULL,
9024825867763712ULL,
18049651735527424ULL,
70506452221952ULL,
275449643008ULL,
9733406720ULL,
2216342585344ULL,
567382630203392ULL,
1134765260406784ULL,
4512412933816832ULL,
9024825867633664ULL,
18049651768822272ULL,
70515108615168ULL,
2491752130560ULL,
567383701868544ULL,
1134765256220672ULL,
2269530512441344ULL,
2256206450263040ULL,
4512412900526080ULL,
9024834391117824ULL,
18051867805491712ULL,
637888545440768ULL,
1135039602493440ULL,
2269529440784384ULL,
4539058881568768ULL,
1128098963916800ULL,
2256197927833600ULL,
4514594912477184ULL,
9592139778506752ULL,
19184279556981248ULL,
2339762086609920ULL,
4538784537380864ULL,
9077569074761728ULL,
562958610993152ULL,
1125917221986304ULL,
2814792987328512ULL,
5629586008178688ULL,
11259172008099840ULL,
22518341868716544ULL,
9007336962655232ULL,
18014673925310464ULL,
2216338399232ULL,
4432676798464ULL,
11064376819712ULL,
22137335185408ULL,
44272556441600ULL,
87995357200384ULL,
35253226045952ULL,
70506452091904ULL,
567382630219776ULL,
1134765260406784ULL,
2832480465846272ULL,
5667157807464448ULL,
11333774449049600ULL,
22526811443298304ULL,
9024825867763712ULL,
18049651735527936ULL




};


constexpr int rookShifts[64] = {
  52, 53, 53, 53, 53, 53, 53, 52,
  53, 54, 54, 54, 54, 54, 54, 53,
  53, 54, 54, 54, 54, 54, 54, 53,
  53, 54, 54, 54, 54, 54, 54, 53,
  53, 54, 54, 54, 54, 54, 54, 53,
  53, 54, 54, 54, 54, 54, 54, 53,
  53, 54, 54, 54, 54, 54, 54, 53,
  52, 53, 53, 53, 53, 53, 53, 52
};

constexpr int bishopShifts[64] = {
  58, 59, 59, 59, 59, 59, 59, 58,
  59, 59, 59, 59, 59, 59, 59, 59,
  59, 59, 57, 57, 57, 57, 59, 59,
  59, 59, 57, 55, 55, 57, 59, 59,
  59, 59, 57, 55, 55, 57, 59, 59,
  59, 59, 57, 57, 57, 57, 59, 59,
  59, 59, 59, 59, 59, 59, 59, 59,
  58, 59, 59, 59, 59, 59, 59, 58
};


#endif
