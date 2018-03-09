open Jest;

open Game;

open Board;

open Coord;

open Expect;

describe("Board", () => {
  test("length", () =>
    expect(List.length(empty().cells)) |> toEqual(64)
  );
  test("empty", () =>
    expect(List.for_all(c => c == Empty, empty().cells)) |> toBe(true)
  );
  test("default", () => {
    let board = empty() |> default;
    expect(board.cells |> List.filter(cell => cell != Empty) |> List.length)
    |> toBe(32);
  });
  test("makeMove", () => {
    let board = empty() |> default;
    let board' = board |> makeMove({prev: Coord(0, 1), next: Coord(0, 2)});
    let prev = board' |> at(Coord(0, 1));
    let next = board' |> at(Coord(0, 2));
    expect((prev, next)) |> toEqual((Empty, Occupied(Black, Pawn)));
  });
  describe("Winner", () => {
    let board = empty() |> default;
    test("default", () =>
      expect(winner(board)) |> toBe(None)
    );
    test("White", () => {
      let board' =
        board |> makeMove({prev: Coord(3, 0), next: Coord(4, 0)});
      expect(winner(board')) |> toEqual(Some(White));
    });
    test("Black", () => {
      let board' =
        board |> makeMove({prev: Coord(3, 7), next: Coord(4, 7)});
      expect(winner(board')) |> toEqual(Some(Black));
    });
  });
});