#ifndef RATIONAL_H
#define RATIONAL_H

class rational {
    int numer;
    int denom;
    int calcGCD(int l, int r) const;

public:
    rational(int numer);
    int getNum() const;
    int getDenom() const;
    rational(int numer, int denom);
    rational const operator -(rational const &) const;
    rational const operator +(rational const &) const;
    rational const operator *(rational const &) const;
    rational const operator /(rational const &) const;
};

#endif
