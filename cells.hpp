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

const char* lwssSynthRLE = R"""(
#CSYNTH xq4_6frc costs 3 gliders (true).
#CLL state-numbering golly
x = 8, y = 10, rule = B3/S23
4bo$3bo$3b3o4$6bo$3o2b2o$2bo2bobo$bo!
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

const char* lobsterRLE = R"""(
#N Lobster (spaceship)
#O Matthias Merzenich
#C https://conwaylife.com/wiki/Lobster_(spaceship)
#C https://conwaylife.com/patterns/83p7h1v1.rle
x = 26, y = 26, rule = B3/S23
11b3o$13bo$8b2o2bo$8b2o$12b2o$11b2o$10bo2bo2$8bo2bo$7bo3bo$6bob3o$5bo$
5bo13bobo2b2o$6bo13b2obobo$b2o13b2o2bo4bo$o2b2o2b2o6bo3bo$5bo2bo6bo6b
2o$9b2o4bobo4b2o$2bo3bo3bo5bo$6b2o4bo2bo$bobo5bo3b2o$2o8bo$5bo4bo$7bo
3bo$4b2o5bo$4bo5bo!
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