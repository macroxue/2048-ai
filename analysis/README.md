# 2048 analysis

This directory contains utility programs for calculating best moves and their
probabilities under two strategies: the well-known Snake Chain and the lesser-known
[Perimeter Defense Formation](https://2048masters.com/lessons/pdf/training-1/index.html).

Here are the commands to get all lookup tables in just a minute.
```
make
./snake8 64 > table.snake8.64
./snake8 128 > table.snake8.128
./snake8 256 > table.snake8.256
./pdf8 64 > table.pdf8.64
./pdf8 128 > table.pdf8.128
./pdf8 256 > table.pdf8.256
```

There is also a DPDF version. Since DPDF has too many positions, the usage is
changed to online query instead and the lookup tables are not saved to files.
You enter a position at the ">" prompt and the program outputs the best move
and its probability of success. When you're done, type Control-C or Control-D
to exit the program. Warning: DPDF-256 requires 2GB of free memory and DPDF-512
7GB.
```
sudo apt install libreadline-dev
make dpdf
./dpdf 64
./dpdf 128
./dpdf 256
./dpdf 512
```

## Snake Chain

Below are a few examples for using the lookup tables with Snake Chain strategy.
Suppose you are at this position trying to figure out how to get a new 64 tile to
reach 16384.
```
---------------------------------
|  1024 |  2048 |  4096 |  8192 |
---------------------------------
|   512 |   256 |   128 |    64 |
---------------------------------
|     2 |     2 |     2 |     4 |
---------------------------------
|     0 |     0 |     2 |     2 |
---------------------------------
```
To find out the best move, look for the line below in table.snake8.64.
```
2 2 2 4 0 0 2 2, right, 0.976849
```
This line basically says that the best move is to the right and the chance of
getting a new 64 tile is about 97.7%, which is very high.

Let's turn to a harder situation where your goal is to get a new 128 tile
and then the noble 32768 tile.
```
---------------------------------
| 16384 |  4096 |  1024 |   256 |
---------------------------------
|  8192 |  2048 |   512 |   128 |
---------------------------------
|     2 |     2 |     2 |     4 |
---------------------------------
|     0 |     0 |     2 |     2 |
---------------------------------
```
In table.snake8.128, there is a similar line for moving to the right but the
chance for a new 128 tile has reduced to 78.2%.
```
2 2 2 4 0 0 2 2, right, 0.782384
```

Now let's see how the Snake Chain is doing for reaching the majestic 65536 tile,
where a new 256 tile is needed from the lower 2x4 space.
```
---------------------------------
| 32768 |  8192 |  2048 |   512 |
---------------------------------
| 16384 |  4096 |  1024 |   256 |
---------------------------------
|     2 |     2 |     2 |     4 |
---------------------------------
|     0 |     0 |     2 |     2 |
---------------------------------
```
Well, table.snake8.256 has a bad news. The best move is still to the right but the
chance of success has dropped to an abysmal 12.5%.
```
2 2 2 4 0 0 2 2, right, 0.124619
```

## Perimeter Defense Formation

Now let's look at Perimeter Defense Formation (PDF) to see if it can do better.
Below is the first position for getting a new 64 tile at (3, 3) which is
currently occupied by a 4 tile.
```
---------------------------------
|  8192 |  2048 |   512 |     0 |
---------------------------------
|  4096 |  1024 |   256 |     0 |
---------------------------------
|   128 |    64 |     4 |     2 |
---------------------------------
|     2 |     2 |     2 |     2 |
---------------------------------
```
In table.pdf8.64, there is such a line saying that the best move is to go up
and the rate of success is 99.1%. This is slightly better than Snake Chain's
97.7%.
```
0 0 4 2 2 2 2 2, up, 0.990791
```

So how about the position of getting a new 128 tile for the noble 32768?
```
---------------------------------
| 16384 |  8192 |  1024 |     0 |
---------------------------------
|  4096 |  2048 |   512 |     0 |
---------------------------------
|   256 |   128 |     4 |     2 |
---------------------------------
|     2 |     2 |     2 |     2 |
---------------------------------
```
Looking into table.pdf8.128, one can find out that the success rate of going up
is 81.2%, 3 points higher than Snake Chain's 78.2%.
```
0 0 4 2 2 2 2 2, up, 0.811834
```

The last position is for a new 256-tile and the majestic 65536.
```
---------------------------------
| 32768 | 16384 |  8192 |     0 |
---------------------------------
|  4096 |  2048 |  1024 |     0 |
---------------------------------
|   512 |   256 |     4 |     2 |
---------------------------------
|     2 |     2 |     2 |     2 |
---------------------------------
```
The definitive answer from table.pdf8.256 is going up with 17.3% chance of
success, which is almost 5 points higher than Snake Chain's 12.5%.
```
0 0 4 2 2 2 2 2, up, 0.172561
```

## Conclusions

From examples above, it's clear that PDF is better than Snake Chain. However,
it doesn't mean that players should switch to PDF, especially when the goal
is to reach 32768. I personally find Snake Chain easier to grasp than PDF.
For humans, it's more important not to make mistakes so a simpler strategy
can actually be better.
