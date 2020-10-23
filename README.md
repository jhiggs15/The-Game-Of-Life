# The-Game-Of-Life
### John Conways game of life with threads! Coded in C++ for CS 3013 (Operating Systems) at WPI
  
  
#### The following is the project statement provided
The Game of Life was invented by John Conway. The original article describing the game can
be found in the April 1970 issue of Scientific American (http://www.sciam.com/), page
120. The game is played on a grid of cells, each of which has eight neighbors (adjacent cells).
cell is either occupied (by an organism) or not. For boundary cases, assume cells outside of the
grid are unoccupied. The rules for deriving a generation from the previous one are these:

- Death. If an occupied cell has 0, 1, 4, 5, 6, 7, or 8 occupied neighbors, the organism dies (0
or 1 of loneliness; 4 thru 8 of overcrowding).
- Survival. If an occupied cell has two or three neighbors, the organism survives to the next
generation.
- Birth. If an unoccupied cell has three occupied neighbors, it becomes occupied.

Once started with an initial configuration of organisms (Generation 0), the game continues
from one generation to the next until a predefined number of generations is reached.

Use multiple threads to perform the work. This approach could improve performance on a multiprocessor, but introduces added complexity on synchronizing the activities of the threads.
You will use thread 0 to coordinate the activities of one or more worker threads, which are
actually doing the work of playing the game. Each worker thread computes the new generation
of the game for an assigned range of rows as initially specified by thread 0. The number of rows
assigned to each thread by thread 0 should be roughly equal. After each generation, each thread
reports results back to thread 0 and waits for a GO message from thread 0 before continuing to
the next generation.

