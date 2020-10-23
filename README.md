# The-Game-Of-Life
### John Conways game of life with threads!\
### Coded in C++ for CS 3013 (Operating Systems)


The Game of Life was invented by John Conway. The original article describing the game can
be found in the April 1970 issue of Scientific American (http://www.sciam.com/), page
120. The game is played on a grid of cells, each of which has eight neighbors (adjacent cells).
cell is either occupied (by an organism) or not. For boundary cases, assume cells outside of the
grid are unoccupied. The rules for deriving a generation from the previous one are these:

- Death. If an occupied cell has 0, 1, 4, 5, 6, 7, or 8 occupied neighbors, the organism dies (0
or 1 of loneliness; 4 thru 8 of overcrowding).
- Survival. If an occupied cell has two or three neighbors, the organism survives to the next
generation.\
- Birth. If an unoccupied cell has three occupied neighbors, it becomes occupied.

Once started with an initial configuration of organisms (Generation 0), the game continues
from one generation to the next until a predefined number of generations is reached.
The straightforward way in which to implement the program is to maintain two separate twodimensional arrays for even and odd generations. These can be maintained as globalvariables.
