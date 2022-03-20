# 2048 - ai

This is a strong AI for the popular game 2048. It reaches the 65536 tile 3% of the
time, of course, without undos. To my knowledge, this is the first AI that "easily"
reaches the 65536 tile. Previously [1] reported 1 out of 10,000 attempts.

Below is a screenshot with [2048 clone](https://github.com/macroxue/2048-clone)
when the AI was about to build a 32768 tile next to the 65536 tile but died
unfortunately. In text mode, the AI also achieved the high score of 1556664
with a 65536 tile and a 32768 tile. Read on to see how to recreate the high
score.

![High Score](https://github.com/macroxue/2048-ai/blob/master/highscore.png)

## Performance

On Intel Xeon 2.3GHz CPU

|depth |games |score/game|% 65536|% 32768|% 16384|% 8192|moves/s|seconds/game|
|------|------|----------|-------|-------|-------|------|-------|------------|
|3     | 1000 |  417381  | 1.0   | 37.8  | 77.4  | 91.6 | 5000  |3   |
|4     | 1000 |  527424  | 1.6   | 55.3  | 87.4  | 94.9 | 1500  |12  |
|5     | 1000 |  589718  | 1.3   | 65.0  | 92.5  | 98.2 | 500   |42  |
|6     | 500  |  622000  | 1.8   | 68.4  | 96.0  | 99.6 | 150   |130 |
|7     | 200  |  642571  | 3.0   | 74.0  | 97.5  | 99.0 | 50    |430 |

The above numbers can be recreated with this command:
```
./2048 -d <depth> -i <games> 2001
```
The AI is considerably stronger and faster than the previous best at [2], which
reported 69% rate of reaching the 32768 tile but never the 65536 tile.


## System requirements

 * A Linux environment with G++ compiler supporting c++0x or above.
 * 8GB of memory
 * 4GB of free disk (SSD preferred)

## How to build
```
make
```

## How to run

Be patient for the very first run. It can take 20 to 30 minutes to compute and
save two lookup tables, depending on the speed of the system. Later runs are
much faster by loading the tables within a few seconds.

Below are some examples.
```
# Play a random game with 3-ply search.
./2048 -d3

# Play game# 2050 with 3-ply search. You will see the 65536 tile.
./2048 -d3 2050

# Play game# 2050 with 3-ply search in verbose mode showing all moves.
./2048 -d3 -v 2050

# Play 10 games starting game# 2050 with 3-ply search.
./2048 -d3 -i10 2050

# Play game# 2833 with 7-ply search. You will see the 1556664 high score.
./2048 -d7 2833
```

### Interactive mode

```
# Play an interactive game in text mode, with the AI suggesting moves.
./2048 -I
```

In interactive mode, "Space" is the key to accept AI suggestions or you can make
moves with other prompted keys. It's fun to mess up with the AI and see how it
recovers or dies trying.


### Server mode

```
# Run the AI in server mode on port 8080 and enable interactive mode.
./2048 -S 8080 -I
```

The AI can also run in server mode to suggest moves and analyze user moves.
For example, this [2048 clone](https://github.com/macroxue/2048-clone) sends
GET requests like "http://localhost:8080/move?board=EDC1BA9187611111" to the
AI server and receives one-character replies like 'u', 'l', 'r', 'd' and 'g',
which stand for up, left, right, down and game-over respectively. Then the
clone auto-plays the move suggestions until the game ends.

To try the clone and the AI together,
 * download this repository;
 * build the AI and run it in server mode according to instructions above;
 * wait until the server shows "Server ready";
 * open [this link](https://macroxue.github.io/2048-clone) to play.

The game can auto-play continuously or step by step. The user can still control
the game with arrow keys and such moves are sent to the AI for analysis.  When
a user move is significantly worse than the move that the AI would take, the
server will point out the better move in the terminal. The sensitivity is set
to 0.9 by default and can be changed by the `-O` flag.


### Training with Snake Chain

Most human players follow [Snake Chain Formation](https://2048masters.com/lessons/)
because it's easy to grasp, while the AI plays
[Perimeter Defense Formation](https://2048masters.com/lessons/pdf/training-1/index.html).
which is superior to Snake Chain.

For training purpose, the AI can also handle Snake Chain with the following
command.

```
# Run the AI in server mode on port 8080 and enable training with Snake Chain.
./2048 -S 8080 -T -v
```

In training mode, the AI conducts exhaustive Expectimax search on the very
first board and keeps all intermediate boards for future lookups. It may take a
few minutes to finish the search. The AI can't handle all boards optimally but
its success rate is 86% with boards like below where x's are small tiles.

```
---------------------------------
| 16384 |  8192 |  4096 |  2048 |
---------------------------------
|  1024 |   512 |   256 |     x |
---------------------------------
|     x |     x |     x |     x |
---------------------------------
|     x |     x |     x |     x |
---------------------------------
```

If you want to change the starting board, the AI must be restarted so it can
recompute the lookup table for the new starting board. Also be aware that the
AI may consume a ton of memory when the board has more small tiles than the
example above.

## How it works

The AI has two components.
 * Expectimax search using a relatively simple evaluation function for the board.
   This can be slow due to exploring a vast search space and its strength
   depends on the search depth.
 * Near-optimal lookup for the next move when the board has certain features.
   This is instantaneous and handles the most difficult situations when large tiles
   occupy the board.

The two components work in tandem. The search drives large tiles to the top-left
corner and the lookup figures out the moves to get the next large tile.

Take the board below for example. We need a new 64 tile so the large tiles can
be merged into a 32768 tile.
```
---------------------------------
| 16384 |  8192 |  1024 |     2 |
---------------------------------
|  4096 |  2048 |   512 |     2 |
---------------------------------
|   256 |   128 |    64 |     2 |
---------------------------------
|     2 |     2 |     2 |     2 |
---------------------------------
```
The goal is to get the new 64 tile right next to the existing 64 tile while
moving only the bottom row and the right-most column. It turns out that this
can be achieved nearly 80% of the time. Why? Because one can apply the
Expectimax algorithm until either the goal or a deadend is reached. This is
usually infeasible during search because it may take many moves to reach the
goal or a deadend, way beyond the search depth.  However, with dynamic
programming, this thorough exploration needs only to be done once. The best
move for this board and the moves for all intermediate boards during
the exploration are saved for future lookups.

The same idea is extended to have two moving rows and one moving column so
building the next 512 tile can be from lookups as well. This greatly improves
the strength and the speed of the AI, at the cost of more memory and more time
in computing the lookup tables.  Right now, the AI uses roughly 6GB of memory
to compute the tables and 3.2GB after that.

### Analysis

Here is an analysis [comparing Snake Chain and Perimeter Defense Formation
strategies](https://github.com/macroxue/2048-ai/tree/master/analysis) with
optimal moves, which are produced by utility programs based on the same
exhaustive Expectimax algorithm.


## Potential improvements

 * The search component has some weakness. Occasionally it gets stuck with the
2048 tile or even lower.
 * Multiple threads can speed up computing the lookup tables.
 * In theory the lookup can be further extended to have two moving rows and two
moving columns, given hundreds GB of memory.

## References
[1] K. H. Yeh, I. C. Wu, C. H. Hsueh, C. C. Chang, C. C. Liang, and
H. Chiang. Multi-stage temporal difference learning for 2048-like
games. IEEE Transactions on Computational Intelligence and AI in
Games, 2016.

[2] https://github.com/aszczepanski/2048.
