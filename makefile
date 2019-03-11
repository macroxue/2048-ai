all: 2048.p 2048 2048.g board_test

SOURCES=board.cc
HEADERS=node.h board.h cache.h tuple_move.h tuple.h snake.h plan.h
CC_OPTIONS=-std=c++0x -pthread -Wall
RUN_OPTIONS=-d2 -T

2048.p: 2048.cc $(SOURCES) $(HEADERS)
	g++ $(CC_OPTIONS) -O3 -fprofile-generate -o $@ 2048.cc $(SOURCES)
	time ./$@ $(RUN_OPTIONS)
2048: 2048.cc $(SOURCES) $(HEADERS)
	g++ $(CC_OPTIONS) -O3 -fprofile-use -o $@ 2048.cc $(SOURCES)
	time ./$@ $(RUN_OPTIONS)
2048.g: 2048.cc $(SOURCES) $(HEADERS)
	g++ $(CC_OPTIONS) -g -o $@ 2048.cc $(SOURCES)
board_test: board_test.cc board.cc board.h
	g++ -std=c++0x -g -o $@ board_test.cc board.cc
	./$@
compact_map_test: compact_map_test.cc compact_map.h
	g++ -std=c++0x -g -o $@ compact_map_test.cc
	./$@
