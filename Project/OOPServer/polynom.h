#pragma once
#include "number.h"
#include "Array.h"
#include <iostream>

class QString;

class Polynom
{
private:
    Array* roots;
    Array* coefficients;
    number an;
    int degree;

    void calculateCoefficients();

public:
    Polynom();
    Polynom(int n, number leadingCoeff);
    ~Polynom();

    void inputPolynomial();
    void changeLeadingCoefficient(number newAn);
    void changeRoot(int index, number newRoot);
    number calculateValue(number x);
    void resize(int newDegree);

    void printFactorForm();
    void printStandardForm();

    int getDegree() const;
    Array* getRoots() const;
    Array* getCoefficients() const;
    number getLeadingCoefficient() const;

    friend QString& operator<<(QString& s, const Polynom& p);

    friend QString& printFactorForm(QString& s, const Polynom& p);
    friend QString& printStandardForm(QString& s, const Polynom& p);

};
