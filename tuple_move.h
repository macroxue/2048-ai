#ifndef TUPLE_MOVE_H
#define TUPLE_MOVE_H

struct TupleMove {
  float Prob() const { return prob * float(1.0 / 16000); }
  bool ValidProb() const { return prob <= 16000; }
  void SetProb(float p) { prob = p * float(16000.0); }

  unsigned short move : 2;

 private:
  unsigned short prob : 14;
};

struct CompressedTupleMove {
  float Prob() const { return prob * float(1.0 / 60); }
  bool ValidProb() const { return prob <= 60; }
  void SetProb(float p) {
    prob = p * float(60.0);
    if (p > 0 && prob == 0) prob = 1;
  }

  unsigned char move : 2;

 private:
  unsigned char prob : 6;
};

#endif
