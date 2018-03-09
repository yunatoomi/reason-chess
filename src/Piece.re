open Coord;

let isWalkable =
    (board: Game.board, player: Game.player, coord: Game.coord)
    : bool =>
  isValid(coord)
  && (
    switch (board |> at(coord)) {
    | Empty => true
    | Occupied(p, _) => p != player
    }
  );

let allTheWay =
    (
      board: Game.board,
      player: Game.player,
      direction: Game.direction,
      coord: Game.coord,
    )
    : list(Game.coord) => {
  let rec allTheWay' =
          (
            board: Game.board,
            player: Game.player,
            direction: Game.direction,
            coord: Game.coord,
            steps: list(Game.coord),
          )
          : list(Game.coord) => {
    let step = coord |> next(direction);
    if (isValid(step)) {
      switch (board |> at(step)) {
      | Game.Occupied(plyr, _) when plyr == player => steps
      | Game.Occupied(_, _) => [step, ...steps]
      | Game.Empty =>
        allTheWay'(board, player, direction, step, [step, ...steps])
      };
    } else {
      steps;
    };
  };
  allTheWay'(board, player, direction, coord, []);
};

let allTheWays =
    (
      board: Game.board,
      player: Game.player,
      directions: list(Game.direction),
      coord: Game.coord,
    )
    : list(Game.coord) => {
  let atw = allTheWay(board, player);
  directions |> List.fold_left((acc, dir) => acc @ atw(dir, coord), []);
};

module type PieceParam = {
  let possibleMoves:
    (Game.coord, Game.board, Game.player) => list(Game.coord);
};

module type PieceImpl = {
  let possibleMoves: (Game.coord, Game.board, Game.player) => list(Game.move);
};

module Make = (PP: PieceParam) : PieceImpl => {
  let possibleMoves =
      (coord: Game.coord, board: Game.board, player: Game.player)
      : list(Game.move) => {
    let coords = PP.possibleMoves(coord, board, player);
    coords |> List.map(next => ({prev: coord, next}: Game.move));
  };
};