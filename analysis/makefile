all: snake8 pdf8

HEADERS=../board.h ../tuple_move.h tuple.h
CC_OPTIONS=-std=c++0x -pthread -O3

snake8: snake8.cc ../board.cc $(HEADERS)
	g++ $(CC_OPTIONS) -o $@ snake8.cc ../board.cc -DPRECISE_PROB
pdf8: pdf8.cc ../board.cc $(HEADERS)
	g++ $(CC_OPTIONS) -o $@ pdf8.cc ../board.cc -DPRECISE_PROB
dpdf: dpdf.cc ../board.cc $(HEADERS)
	g++ $(CC_OPTIONS) -o $@ dpdf.cc ../board.cc -lreadline
