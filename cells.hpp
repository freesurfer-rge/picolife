const char* gliderCells = R"""(
!Name: Glider
!Author: Richard K. Guy
!The smallest, most common, and first discovered spaceship.
!www.conwaylife.com/wiki/index.php?title=Glider
.O
..O
OOO
)""";

const char* gliderRLE = R"""(
#N Glider
#O Richard K. Guy
#C The smallest, most common, and first discovered spaceship. Diagonal, has period 4 and speed c/4.
#C www.conwaylife.com/wiki/index.php?title=Glider
x = 3, y = 3, rule = B3/S23
bob$2bo$3o!
)""";

const char* lightweightSpaceshipCells = R"""(
!Name: LWSS
!Author: John Conway
!The smallest known orthogonally moving spaceship, and the second most common spaceship (after the glider).
!www.conwaylife.com/wiki/index.php?title=Lightweight_spaceship
.O..O
O
O...O
OOOO
)""";

const char* dartCells = R"""(
!Name: Dart
!Author: David Bell
!A period 3 spaceship with speed c/3 that was found in May 1992.
!www.conwaylife.com/wiki/index.php?title=Dart
.......O
......O.O
.....O...O
......OOO

....OO...OO
..O...O.O...O
.OO...O.O...OO
O.....O.O.....O
.O.OO.O.O.OO.O
)""";

const char* snarkCells = R"""(
! Snark
! Mike Playle
! The fastest and smallest 90-degree stable glider reflector (as of
! June 2013).
! www.conwaylife.com/wiki/Snark
......OO...OO....
......OO..O.OOO..
..........O....O.
......OOOO.OO..O.
......O..O.O.O.OO
.........O.O.O.O.
..........OO.O.O.
..............O..
.................
OO...............
.O.......OO......
.O.O.....OO......
..OO.............
.................
.................
.................
.................
.................
.................
............OO...
...OO.......O....
..O.O........OOO.
....O..........O.
)""";

const char* coeShipCells = R"""(
!Name: Coe ship
!Author: Tim Coe
!A puffer engine discovered in October 1995.
....OOOOOO
..OO.....O
OO.O.....O
....O...O
......O
......OO
.....OOOO
.....OO.OO
.......OO
)""";

const char* fireShipCells = R"""(
! Fireship
! Simon Ekström
! https://conwaylife.com/wiki/Fireship
! https://conwaylife.com/patterns/fireship.cells
....O..O....
....O..O....
...O....O...
...O....O...
...O....O...
...O....O...
..OOO..OOO..
..O.OOOO.O..
...O....O...
............
............
.....OO.....
.....OO.....
.....OO.....
...O....O...
.OOO....OOO.
OOO......OOO
OO........OO
.OOOOOOOOOO.
..OOOOOOOO..
....OOOO....
)""";

const char* lobsterCells = R"""(
! Lobster (spaceship)
! Matthias Merzenich
! https://conwaylife.com/wiki/Lobster_(spaceship)
! https://conwaylife.com/patterns/83p7h1v1.cells
...........OOO............
.............O............
........OO..O.............
........OO................
............OO............
...........OO.............
..........O..O............
..........................
........O..O..............
.......O...O..............
......O.OOO...............
.....O....................
.....O.............O.O..OO
......O.............OO.O.O
.OO.............OO..O....O
O..OO..OO......O...O......
.....O..O......O......OO..
.........OO....O.O....OO..
..O...O...O.....O.........
......OO....O..O..........
.O.O.....O...OO...........
OO........O...............
.....O....O...............
.......O...O..............
....OO.....O..............
....O.....O...............
)""";

const char* achimsp144Cells = R"""(
!Name: Achim's p144
!Author: Achim Flammenkamp
!Period 144 oscillator found in 1994.
OO........................OO
OO........................OO
..................OO
.................O..O
..................OO
..............O
.............O.O
............O...O
............O..O

............O..O
...........O...O
............O.O
.............O
........OO
.......O..O
........OO
OO........................OO
OO........................OO
)""";


const char* queenBeeShuttleCells = R"""(
!Name: Queen bee shuttle
!Author: Bill Gosper
!A period 30 oscillator.
!www.conwaylife.com/wiki/index.php?title=Queen_bee_shuttle
.........O
.......O.O
......O.O
OO...O..O...........OO
OO....O.O...........OO
.......O.O
.........O
)""";