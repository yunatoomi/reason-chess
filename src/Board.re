open Game;

open Coord;

module PawnPiece = Piece.Make(Pawn);

module RookPiece = Piece.Make(Rook);

module KnightPiece = Piece.Make(Knight);

module BishopPiece = Piece.Make(Bishop);

module QueenPiece = Piece.Make(Queen);

module KingPiece = Piece.Make(King);

exception InvalidMove(string);

let side = 8;

let iterate = (board: board) : list(cell) => board.cells;

let empty = () : board => {
  let cells = Array.make(side * side, Empty) |> Array.to_list;
  {
    blackCastling: false,
    whiteCastling: false,
    underCheck: None,
    capturedPieces: [],
    cells,
  };
};

let default = (board: board) : board => {
  let cells =
    board
    |> iterate
    |> List.mapi((idx, _) =>
         switch (idx, false) {
         | (0 | 7, _) => Occupied(Black, Rook)
         | (1 | 6, _) => Occupied(Black, Knight)
         | (2 | 5, _) => Occupied(Black, Bishop)
         | (3, _) => Occupied(Black, Queen)
         | (4, _) => Occupied(Black, King)
         | (8 | 9 | 10 | 11 | 12 | 13 | 14 | 15, _) => Occupied(Black, Pawn)
         | (48 | 49 | 50 | 51 | 52 | 53 | 54 | 55, _) =>
           Occupied(White, Pawn)
         | (59, _) => Occupied(White, Queen)
         | (60, _) => Occupied(White, King)
         | (56 | 63, _) => Occupied(White, Rook)
         | (57 | 62, _) => Occupied(White, Knight)
         | (58 | 61, _) => Occupied(White, Bishop)
         | _ => Empty
         }
       );
  {...board, cells};
};

let makeMove = ({prev, next}: move, board: board) => {
  let prevIndex = indexOfCoord(prev);
  let nextIndex = indexOfCoord(next);
  let cells =
    board
    |> iterate
    |> List.mapi((index, cell) =>
         switch (index) {
         | idx when idx == prevIndex && cell != Empty => Empty
         | idx when idx == prevIndex =>
           raise(InvalidMove("cant move empty cell"))
         | idx when idx == nextIndex => board |> at(prev)
         | _ => cell
         }
       );
  {...board, cells};
};

let setAt = (piece: piece, player: player, coord: coord, board: board) : board => {
  let setIndex = indexOfCoord(coord);
  let cells =
    board
    |> iterate
    |> List.mapi((index, cell) =>
         switch (index) {
         | idx when idx == setIndex => Occupied(player, piece)
         | _ => cell
         }
       );
  {...board, cells};
};

let possibleMoves = (coord: coord, board: board) : list(move) => {
  let cell = board |> at(coord);
  switch (cell) {
  | Occupied(player, Pawn) => PawnPiece.possibleMoves(coord, board, player)
  | Occupied(player, Rook) => RookPiece.possibleMoves(coord, board, player)
  | Occupied(player, Knight) =>
    KnightPiece.possibleMoves(coord, board, player)
  | Occupied(player, Bishop) =>
    BishopPiece.possibleMoves(coord, board, player)
  | Occupied(player, Queen) => QueenPiece.possibleMoves(coord, board, player)
  | Occupied(player, King) => KingPiece.possibleMoves(coord, board, player)
  | _ => []
  };
};

let winner = (board: board) : option(player) =>
  board
  |> iterate
  |> List.fold_left(
       (acc, cell) =>
         switch (acc, cell) {
         | (None, Occupied(player, King)) => Some(player)
         | (Some(_), Occupied(_, King)) => None
         | _ => acc
         },
       None,
     );