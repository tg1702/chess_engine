
import asyncio
import asyncssh
import chess
import chess.engine
import chess.pgn
import collections
"""
async def main() -> None:
    async with asyncssh.connect('localhost') as conn:
        channel, engine = await conn.create_subprocess(chess.engine.UciProtocol, r"../perft")
        await engine.initialize()

        # Play, analyse, ...
        await engine.ping()

asyncio.run(main())

"""
async def main() -> None:

     transport, engine = await chess.engine.popen_uci(r"../perft")
     try:
      board = chess.Board()
    
      while not board.is_game_over():
        result = await engine.play(board, chess.engine.Limit(white_clock=10, black_clock=10))
        board.push(result.move)

      await engine.quit()
      
      
      game = chess.pgn.Game()

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
            board.push(move)

      game.headers["Result"] = board.result()

      print(game)
    
     except chess.engine.EngineTerminatedError as e:
        print(f"Engine terminated with error: {e}")

asyncio.run(main())


