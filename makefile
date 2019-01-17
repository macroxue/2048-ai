all: 2048.p 2048 2048.g board_test

SOURCES=board.cc
HEADERS=node.h board.h cache.h tuple_move.h tuple.h
CC_OPTIONS=-std=c++0x -pthread
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
