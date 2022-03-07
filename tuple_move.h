#ifndef TUPLE_MOVE_H
#define TUPLE_MOVE_H

#ifdef PRECISE_PROB
struct TupleMove {
  float Prob() const { return prob * 1e-9; }
  bool ValidProb() const { return prob <= 1000000000; }
  void SetProb(float p) { prob = p * 1e9; }

  unsigned int move : 2;
  unsigned int prob : 30;
};
#else
struct TupleMove {
  float Prob() const { return prob * float(1.0 / 16000); }
  bool ValidProb() const { return prob <= 16000; }
  void SetProb(float p) { prob = p * float(16000.0); }

  unsigned short move : 2;
  unsigned short prob : 14;
};
#endif

struct CompressedTupleMove {
  float Prob() const { return prob * float(1.0 / 60); }
  bool ValidProb() const { return prob <= 60; }
  void SetProb(float p) {
    prob = p * float(60.0);
    if (p > 0 && prob == 0) prob = 1;
  }

  unsigned char move : 2;
  unsigned char prob : 6;
};

#endif
