# Rooky Chess Engine

A simple chess engine implemented in C++.

## Features

- Move generation
    - Bitboard representation
    - Lookup tables
    - Magic number generation
- Search
    - Negamax with Alpha-Beta pruning
    - Quiescence Search
    - Most Valueable Victim, Least Valueable Attacker move ordering
- Evaluation
    - Negamax search with Alpha-Beta pruning
    - Piece-Square Tables with tapered evaluation

## Getting Started

### Prerequisites
 - C++ 17 or above
 - Linux OS

### Installation

#### Clone or fork this project
```bash
git clone https://github.com/tg1702/chess_engine.git
```

#### Build using ```cmake```

```bash
mkdir build
cd build
```

```
cmake ..
cmake --build .
```
### Usage
This engine is [UCI](https://en.wikipedia.org/wiki/Universal_Chess_Interface) compliant, the most popular interface for chess engine communication. It should work with any GUI that supports this interface.

Here is a list of the UCI commands currently supported:
```bash
uci
isready
position [fen | startpos]
go [wtime | btime | winc | binc]
quit
```

If you only want to play against the engine without using the interface, and you have a Lichess account, click [here](https://lichess.org/@/rooky_engine)  


## Contributing

Contributions are welcome! Please open issues or submit pull requests.

## License



## Acknowledgements

- Chess Programming Wiki: https://www.chessprogramming.org/Main_Page
- Stockfish Github: https://github.com/official-stockfish/Stockfish
- Rustic chess engine: https://www.rustic-chess.org/
- Chess Programming Youtube Channel: https://www.youtube.com/playlist?list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs
- TalkChess Forum: https://talkchess.com/
- https://josherv.in/2021/03/19/chess-1/