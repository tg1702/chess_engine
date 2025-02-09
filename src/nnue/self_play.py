
import asyncio
import random
import asyncssh
import chess
import chess.engine
import chess.pgn
import chess.polyglot
import collections
import struct

MAX_ROUNDS = 1

def convert_score_format(result: str):
   if result == "1-0":
      return "1"
   elif result == "0-1":
      return "0"
   else:
      return "0.5"

def write_board_data_to_file(fen, score):
  fen_bytes = bytes(fen, 'utf-8')
  size = str(len(fen_bytes)) + 's'
  with open('data.bin', 'ab') as file:
    file.write(struct.pack('B',  len(fen)))
    file.write(struct.pack('f', float(score)))
    file.write(struct.pack("@" + size, bytes(fen, 'utf-8')))
    

def read_board_data_from_file():
  with open('data.bin', 'rb') as file:
    for i in range(MAX_ROUNDS):
      fen_length = struct.unpack('B', file.read(1))[0]
      score = struct.unpack('f', file.read(4))[0]
      fens = file.read(fen_length).decode('utf-8')
    

  print(fen_length, fens, score)

def board_to_pgn(board):
  game = chess.pgn.Game()

  res = board.result()
    # Undo all moves.
  switchyard = collections.deque()
  while board.move_stack:
        switchyard.append(board.pop())

  game.setup(board)
  node = game

    # Replay all moves.
  while switchyard:
        move = switchyard.pop()
        node = node.add_variation(move)
        write_board_data_to_file(board.fen(), convert_score_format(res))
        board.push(move)

  game.headers["Result"] = board.result()
  print(game)

def play_book_moves(board, max_book_moves):
  bookMoves = 0
  while bookMoves < max_book_moves:
    
    moves = list(chess.polyglot.open_reader('komodo.bin').find_all(board))  # Get all book moves
    if not moves:
      break  # Exit if no book moves are found

    entry = random.choice(moves)  # Randomly pick one move
    
    board.push(entry.move)  # Play the move
    bookMoves += 1

async def main() -> None:
     draw = 0
     white_won = 0 
     black_won = 0
     result = None
     try:
      
    
      for i in range(MAX_ROUNDS):
        
        transport, engine = await chess.engine.popen_uci(r"../perft")
        board = chess.Board()
        
        play_book_moves(board, 10)
        
        while not board.is_game_over():
          result = await engine.play(board, chess.engine.Limit(depth=4))
          board.push(result.move)

        await engine.quit()
        
        board_to_pgn(board)
        
        if board.result() == "1-0":
          white_won += 1
        elif board.result() == "0-1":
          black_won += 1
        else:
          draw += 1
        

      read_board_data_from_file()
        
      

     except chess.engine.EngineTerminatedError as e:
        print(f"Engine terminated with error: {e}")

     print("Totals: ")
     print("White won : ", white_won)
     print("Black won ", black_won)
     print("Draw ", draw)
asyncio.run(main())


