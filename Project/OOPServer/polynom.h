#pragma once
#include "complex.h"
#include "Array.h"
#include <iostream>

class QString;

template <class number>
class Polynom
{
private:
    Array<number>* roots;
    Array<number>* coefficients;
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
    Array<number>* getRoots() const;
    Array<number>* getCoefficients() const;
    number getLeadingCoefficient() const;

    template <class T>
    friend QString& operator<<(QString& s, const Polynom<T>& p);

    template <class T>
    friend QString& printFactorForm(QString& s, const Polynom<T>& p);

    template <class T>
    friend QString& printStandardForm(QString& s, const Polynom<T>& p);
};


template <class number>
Polynom<number>::Polynom() {
    degree = 0;
    an = number(0);
    roots = nullptr;
    coefficients = new Array<number>();
    coefficients->create(1);
    coefficients->changeElem(0, number(0));
}

template <class number>
Polynom<number>::Polynom(int n, number leadingCoeff) {
    degree = n;
    an = leadingCoeff;
    if (n > 0) {
        roots = new Array<number>();
        roots->create(n);
        coefficients = new Array<number>();
        coefficients->create(n + 1);
    } else {
        roots = nullptr;
        coefficients = new Array<number>();
        coefficients->create(1);
        coefficients->changeElem(0, leadingCoeff);
    }
    calculateCoefficients();
}

template <class number>
Polynom<number>::~Polynom() {
    delete roots;
    delete coefficients;
}

template <class number>
void Polynom<number>::inputPolynomial() {
    std::cout << "# | Введите степень полинома: ";
    std::cin >> degree;

    if (degree < 0) {
        degree = 0;
        std::cout << "! | Степень не может быть отрицательной. " << std::endl;
    }

    std::cout << "# | Введите ведущий коэффициент an: ";
    std::cin >> an;

    if (degree > 0) {
        if (roots != nullptr) {
            delete roots;
        }
        roots = new Array<number>();
        roots->create(degree);

        std::cout << "# | Введите " << degree << " корней полинома:" << std::endl;
        roots->fill();
    } else {
        if (roots != nullptr) {
            delete roots;
            roots = nullptr;
        }
    }

    if (coefficients != nullptr) {
        delete coefficients;
    }
    coefficients = new Array<number>();
    coefficients->create(degree + 1);

    calculateCoefficients();
    std::cout << "# | Полином создан успешно." << std::endl;
}

template <class number>
void Polynom<number>::changeLeadingCoefficient(number newAn) {
    an = newAn;
    calculateCoefficients();
}

template <class number>
void Polynom<number>::changeRoot(int index, number newRoot) {
    if (degree == 0) {
        std::cout << "! | Нельзя изменить корень у 0-полинома." << std::endl;
        return;
    }

    if (index < 0 || index >= degree) {
        std::cout << "! | Индекс корня выходит за границы." << std::endl;
        return;
    }

    roots->changeElem(index, newRoot);
    calculateCoefficients();
}

template <class number>
number Polynom<number>::calculateValue(number x) {
    if (degree == 0) {
        return an;
    }

    number result = an;
    for (int i = 0; i < degree; i++) {
        number root = roots->getElement(i);
        result = result * (x - root);
    }
    return result;
}

template <class number>
void Polynom<number>::resize(int newDegree) {
    if (newDegree < 0) {
        std::cout << "! | Степень не может быть отрицательной." << std::endl;
        return;
    }

    if (newDegree == 0) {
        degree = 0;
        delete roots;
        roots = nullptr;
        delete coefficients;
        coefficients = new Array<number>();
        coefficients->create(1);
        coefficients->changeElem(0, an);
        return;
    }

    if (roots == nullptr) {
        roots = new Array<number>();
        roots->create(newDegree);
        for (int i = 0; i < newDegree; i++) {
            roots->changeElem(i, number(0));
        }
    } else {
        roots->resize(newDegree);
        if (newDegree > degree) {
            for (int i = degree; i < newDegree; i++) {
                roots->changeElem(i, number(0));
            }
        }
    }

    degree = newDegree;
    delete coefficients;
    coefficients = new Array<number>();
    coefficients->create(degree + 1);
    calculateCoefficients();
}

template <class number>
void Polynom<number>::printFactorForm() {
    if (degree == 0) {
        std::cout << "P(x) = " << an << std::endl;
        return;
    }

    std::cout << "P(x) = ";


    if (an != 1) {
        std::cout << "(" << an << ")";
    }

    for (int i = 0; i < degree; i++) {
        number root = roots->getElement(i);
        std::cout << " * (x";

        if (root != 0) {
            std::cout << " - (" << root << ")";
        }
        std::cout << ")";
    }
    std::cout << std::endl;
}

template <class number>
void Polynom<number>::printStandardForm() {
    if (degree == 0) {
        std::cout << "P(x) = " << an << std::endl;
        return;
    }

    std::cout << "P(x) = ";
    bool first = true;

    for (int i = degree; i >= 0; i--) {
        number coeff = coefficients->getElement(i);

        if (coeff == 0) {
            continue;
        }

        if (!first) {
            if (coeff > 0) {
                std::cout << " + ";
            } else {
                std::cout << " ";
            }
        }

        if (i == 0) {
            std::cout << coeff;
        } else if (i == 1) {
            if (coeff == 1) {
                std::cout << "x";
            } else if (coeff == -1) {
                std::cout << "-x";
            } else {
                std::cout << "(" << coeff << ")*x";
            }
        } else {
            if (coeff == 1) {
                std::cout << "x^" << i;
            } else if (coeff == -1) {
                std::cout << "-x^" << i;
            } else {
                std::cout << "(" << coeff << ")*x^" << i;
            }
        }

        first = false;
    }

    if (first) {
        std::cout << "0";
    }
    std::cout << std::endl;
}

template <class number>
void Polynom<number>::calculateCoefficients() {
    if (degree == 0) {
        coefficients->changeElem(0, an);
        return;
    }

    for (int i = 0; i <= degree; i++) {
        coefficients->changeElem(i, number(0));
    }

    Array<number>* temp = new Array<number>();
    temp->create(degree + 1);
    temp->changeElem(0, number(1));

    for (int i = 1; i <= degree; i++) {
        temp->changeElem(i, number(0));
    }

    for (int rootIndex = 0; rootIndex < degree; rootIndex++) {
        number root = roots->getElement(rootIndex);
        Array<number>* newTemp = new Array<number>();
        newTemp->create(degree + 1);

        for (int i = 0; i <= degree; i++) {
            newTemp->changeElem(i, number(0));
        }

        for (int i = 0; i <= degree; i++) {
            number currentCoeff = temp->getElement(i);
            // Используем оператор != вместо getRe() и getIm()
            if (currentCoeff != 0) {
                if (i + 1 <= degree) {
                    number existing = newTemp->getElement(i + 1);
                    newTemp->changeElem(i + 1, existing + currentCoeff);
                }
                number existing = newTemp->getElement(i);
                newTemp->changeElem(i, existing - root * currentCoeff);
            }
        }

        delete temp;
        temp = newTemp;
    }

    for (int i = 0; i <= degree; i++) {
        number coeff = temp->getElement(i);
        coefficients->changeElem(i, an * coeff);
    }

    delete temp;
}

template <class number>
int Polynom<number>::getDegree() const { return degree; }

template <class number>
Array<number>* Polynom<number>::getRoots() const { return roots; }

template <class number>
Array<number>* Polynom<number>::getCoefficients() const { return coefficients; }

template <class number>
number Polynom<number>::getLeadingCoefficient() const { return an; }
