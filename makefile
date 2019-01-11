all: 2048.p 2048 2048.g board_test

SOURCES=board.cc
HEADERS=node.h board.h cache.h tuple_move.h tuple.h
OPTIONS=-d2 -T

2048.p: 2048.cc $(SOURCES) $(HEADERS)
	g++ -std=c++0x -O3 -fprofile-generate -o $@ 2048.cc $(SOURCES)
	time ./$@ $(OPTIONS)
2048: 2048.cc $(SOURCES) $(HEADERS)
	g++ -std=c++0x -O3 -fprofile-use -o $@ 2048.cc $(SOURCES)
	time ./$@ $(OPTIONS)
2048.g: 2048.cc $(SOURCES) $(HEADERS)
	g++ -std=c++0x -g -o $@ 2048.cc $(SOURCES)
board_test: board_test.cc board.cc board.h
	g++ -std=c++0x -g -o $@ board_test.cc board.cc
	./$@
