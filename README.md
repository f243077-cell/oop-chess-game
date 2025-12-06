# Console Chess Game

A fully functional command-line chess game written in C++ with colored piece rendering and complete rule enforcement.

## Features

- **Complete Chess Rules**: All standard chess piece movements (King, Queen, Rook, Bishop, Knight, Pawn)
- **Check & Checkmate Detection**: Automatic detection of check, checkmate, and stalemate conditions
- **Legal Move Validation**: Prevents illegal moves and moves that would leave the king in check
- **Colored Display**: White pieces shown in red, black pieces in blue, on a yellow board background
- **Unicode Symbols**: Beautiful chess piece symbols (♔♕♖♗♘♙ for white, ♚♛♜♝♞♟ for black)
- **Turn-Based Gameplay**: Alternating turns between white and black players

## Files

- `main.cpp` - Main game loop, board management, and game logic
- `Piece.h` - Abstract base class for chess pieces
- `Pieces.cpp` - Implementation of all six chess piece types and their movement rules

## Compilation

### Requirements
- C++ compiler with C++11 support or later
- Windows OS (uses Windows console API for colors)

### Build Command
```bash
g++ -o chess main.cpp Pieces.cpp -std=c++11
```

## How to Play

1. Run the compiled executable:
   ```bash
   chess.exe
   ```

2. Enter moves using algebraic notation (source square then destination square):
   ```
   White move: e2 e4
   Black move: e7 e5
   ```

3. Square notation:
   - Columns: a-h (left to right)
   - Rows: 1-8 (bottom to top)
   - Example: `e2` is column e, row 2

4. The game automatically:
   - Validates all moves
   - Checks for check/checkmate/stalemate
   - Prevents moves that leave your king in check
   - Announces the winner

5. Exit the game with Ctrl+C

## Movement Rules

- **King**: One square in any direction
- **Queen**: Any number of squares horizontally, vertically, or diagonally
- **Rook**: Any number of squares horizontally or vertically
- **Bishop**: Any number of squares diagonally
- **Knight**: L-shaped move (2 squares in one direction, 1 square perpendicular)
- **Pawn**: 
  - Forward one square (or two from starting position)
  - Captures diagonally forward one square

## Game Flow

1. White moves first
2. Players alternate turns
3. Game ends when:
   - **Checkmate**: One player's king is in check with no legal moves (opponent wins)
   - **Stalemate**: Player has no legal moves but is not in check (draw)
   - Player quits (Ctrl+C)

## Example Game Start

```
   a  b  c  d  e  f  g  h
8|♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜|8
7|♟ ♟ ♟ ♟ ♟ ♟ ♟ ♟|7
6| .  .  .  .  .  .  .  . |6
5| .  .  .  .  .  .  .  . |5
4| .  .  .  .  .  .  .  . |4
3| .  .  .  .  .  .  .  . |3
2|♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙|2
1|♖ ♘ ♗ ♕ ♔ ♗ ♘ ♖|1
   a  b  c  d  e  f  g  h

White move: e2 e4
```

## Technical Details

### Architecture
- **Object-Oriented Design**: Uses polymorphism with abstract `Piece` base class
- **Factory Pattern**: Pieces are created through factory functions
- **Board Representation**: 8x8 array of piece pointers
- **Move Validation**: Multi-layer validation including path checking and check prevention

### Key Classes
- `Board`: Manages the chess board state and game rules
- `Piece`: Abstract base class for all chess pieces
- `King`, `Queen`, `Rook`, `Bishop`, `Knight`, `Pawn`: Concrete piece implementations

## Limitations

- No en passant capture
- No castling
- No pawn promotion
- No move history or undo
- Windows-only (due to console color API)

## Future Enhancements

Potential improvements:
- Add special moves (castling, en passant, pawn promotion)
- Move history and notation recording
- Save/load game state
- AI opponent
- Cross-platform support (Linux/Mac)
- Graphical user interface

## License

Free to use and modify.
