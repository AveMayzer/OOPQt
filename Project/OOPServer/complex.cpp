#include "Complex.h"
#include "cmath"
#include <iostream>

using namespace std;

TComplex::TComplex() {
    this->re = 0;
    this->im = 0;
}

TComplex::TComplex(double re) {
    this->re = re;
    this->im = 0;
}

TComplex::TComplex(double re, double im) {
    this->re = re;
    this->im = im;
}

TComplex TComplex::operator+(const TComplex& other) const {
    return TComplex(re + other.re, im + other.im);
}

TComplex TComplex::operator-(const TComplex& other) const {
    return TComplex(re - other.re, im - other.im);
}

TComplex TComplex::operator*(const TComplex& other) const {
    return TComplex(re * other.re - im * other.im, re * other.im + im * other.re);
}

TComplex TComplex::operator/(const TComplex& other) const {
    double denom = other.re * other.re + other.im * other.im;
    return TComplex((re * other.re + im * other.im) / denom, (im * other.re - re * other.im) / denom);
}

TComplex TComplex::operator+=(const TComplex& other) {
    re += other.re;
    im += other.im;
    return *this;
}

TComplex TComplex::operator/=(const TComplex& other) {
    double denom = other.re * other.re + other.im * other.im;
    double new_re = (re * other.re + im * other.im) / denom;
    double new_im = (im * other.re - re * other.im) / denom;
    re = new_re;
    im = new_im;
    return *this;
}

// Операторы сравнения
bool TComplex::operator==(const TComplex& other) const {
    return re == other.re && im == other.im;
}

bool TComplex::operator==(double value) const {
    return re == value && im == 0;
}

bool TComplex::operator!=(const TComplex& other) const {
    return !(*this == other);
}

bool TComplex::operator!=(double value) const {
    return !(*this == value);
}

bool TComplex::operator<(const TComplex& second) const {
    double abs1 = std::sqrt(re * re + im * im);
    double abs2 = std::sqrt(second.re * second.re + second.im * second.im);

    if (abs1 < abs2) return true;
    if (abs1 == abs2) {
        if (std::atan2(im, re) < std::atan2(second.im, second.re)) return true;
    }
    return false;
}

bool TComplex::operator>(const TComplex& second) const {
    double abs1 = std::sqrt(re * re + im * im);
    double abs2 = std::sqrt(second.re * second.re + second.im * second.im);

    if (abs1 > abs2) return true;
    if (abs1 == abs2) {
        if (std::atan2(im, re) > std::atan2(second.im, second.re)) return true;
    }
    return false;
}

double TComplex::getRe() const {
    return this->re;
}

double TComplex::getIm() const {
    return this->im;
}

double TComplex::abs(TComplex* complex) {
    double a = std::pow(complex->getRe(), 2);
    double b = std::pow(complex->getIm(), 2);
    return std::pow(a + b, 0.5);
}

TComplex pow(TComplex complex, double n) {
    double r = complex.abs(&complex);
    double arg = atan2(complex.getIm(), complex.getRe());
    double new_r = std::pow(r, n);
    double new_arg = arg * n;
    return TComplex(new_r * cos(new_arg), new_r * sin(new_arg));
}

std::istream& operator>>(std::istream& in, TComplex& c) {
    in >> c.re >> c.im;
    return in;
}

std::ostream& operator<<(std::ostream& out, const TComplex& c) {
    if (c.im == 0) {
        out << c.re;
        return out;
    }
    if (c.re == 0) {
        out << c.im << "i";
        return out;
    }
    out << c.re << (c.im >= 0 ? "+" : "") << c.im << "i";
    return out;
}
