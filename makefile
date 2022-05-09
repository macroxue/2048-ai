all: 2048 board_test array_test

SOURCES=board.cc
HEADERS=node.h board.h cache.h tuple_move.h tuple.h snake.h plan.h array.h
CC_OPTIONS=-std=c++0x -pthread -Wall

2048: 2048.cc $(SOURCES) $(HEADERS)
	g++ $(CC_OPTIONS) -O3 -o $@ 2048.cc $(SOURCES)

board_test: board_test.cc board.cc board.h
	g++ $(CC_OPTIONS) -g -o $@ board_test.cc board.cc
	./$@

array_test: array_test.cc array.h
	g++ $(CC_OPTIONS) -g -o $@ array_test.cc
	./$@

compact_map_test: compact_map_test.cc compact_map.h
	g++ $(CC_OPTIONS) -g -o $@ compact_map_test.cc
	./$@
