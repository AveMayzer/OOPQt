#pragma once

#include <iostream>
using namespace std;

class TComplex {
public:
    TComplex( );
    TComplex(double re);
    TComplex(double re, double im);

    TComplex operator+(const TComplex& other) const;
    TComplex operator-(const TComplex& other) const;
    TComplex operator*(const TComplex& other) const;
    TComplex operator/(const TComplex& other) const;

    TComplex operator+=(const TComplex& other);
    TComplex operator/=(const TComplex& other);

    double getRe() const;
    double getIm() const;

    double abs(TComplex* complex);

    bool operator<(TComplex& second);
    bool operator>(TComplex& second);

    friend std::istream& operator>>(std::istream& in, TComplex& c);
    friend std::ostream& operator<<(std::ostream& out, const TComplex& c);
    friend TComplex pow(TComplex complex, double n);


private:
    double re = 0;
    double im = 0;
};
