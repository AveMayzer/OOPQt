#include "Polynom.h"
#include <iostream>

using namespace std;

Polynom::Polynom() {
    degree = 0;
    an = number(0, 0);
    roots = nullptr;
    coefficients = new Array();
    coefficients->create(1);
    coefficients->changeElem(0, number(0, 0));
}

Polynom::Polynom(int n, number leadingCoeff) {
    degree = n;
    an = leadingCoeff;
    if (n > 0) {
        roots = new Array();
        roots->create(n);
        coefficients = new Array();
        coefficients->create(n + 1);
    } else {
        roots = nullptr;
        coefficients = new Array();
        coefficients->create(1);
        coefficients->changeElem(0, leadingCoeff);
    }
    calculateCoefficients();
}

Polynom::~Polynom() {
    delete roots;
    delete coefficients;
}

void Polynom::inputPolynomial() {
    cout << "# | Введите степень полинома: ";
    cin >> degree;

    if (degree < 0) {
        degree = 0;
        cout << "! | Степень не может быть отрицательной. " << endl;
    }

    cout << "# | Введите ведущий коэффициент an (действ и мнимая части): ";
    cin >> an;

    if (degree > 0) {
        if (roots != nullptr) {
            delete roots;
        }
        roots = new Array();
        roots->create(degree);

        cout << "# | Введите " << degree << " корней полинома:" << endl;
        cout << "# | Для каждого корня вводите действительную и мнимую части:" << endl;
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
    coefficients = new Array();
    coefficients->create(degree + 1);

    calculateCoefficients();
    cout << "# | Полином создан успешно." << endl;
}

void Polynom::changeLeadingCoefficient(number newAn) {
    an = newAn;
    calculateCoefficients();
}

void Polynom::changeRoot(int index, number newRoot) {
    if (degree == 0) {
        cout << "! | Нельзя изменить корень у 0-полинома." << endl;
        return;
    }

    if (index < 0 || index >= degree) {
        cout << "! | Индекс корня выходит за границы." << endl;
        return;
    }

    roots->changeElem(index, newRoot);
    calculateCoefficients();
    cout << "# | Корень изменен успешно." << endl;
}

number Polynom::calculateValue(number x) {
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

void Polynom::resize(int newDegree) {
    if (newDegree < 0) {
        cout << "! | Степень не может быть отрицательной." << endl;
        return;
    }

    if (newDegree == 0) {
        degree = 0;
        delete roots;
        roots = nullptr;
        delete coefficients;
        coefficients = new Array();
        coefficients->create(1);
        coefficients->changeElem(0, an);
        return;
    }

    if (roots == nullptr) {
        roots = new Array();
        roots->create(newDegree);
        for (int i = 0; i < newDegree; i++) {
            roots->changeElem(i, number(0, 0));
        }
    } else {
        roots->resize(newDegree);
        if (newDegree > degree) {
            for (int i = degree; i < newDegree; i++) {
                roots->changeElem(i, number(0, 0));
            }
        }
    }

    degree = newDegree;

    delete coefficients;
    coefficients = new Array();
    coefficients->create(degree + 1);

    calculateCoefficients();
}

void Polynom::printFactorForm() {
    if (degree == 0) {
        cout << "P(x) = " << an << endl;
        return;
    }

    cout << "P(x) = ";

    if (!(an.getRe() == 1 && an.getIm() == 0)) {
        cout << "(" << an << ")";
    }

    for (int i = 0; i < degree; i++) {
        number root = roots->getElement(i);
        cout << " * (x";

        if (root.getRe() != 0 || root.getIm() != 0) {
            cout << " - (" << root << ")";
        }
        cout << ")";
    }
    cout << endl;
}

void Polynom::printStandardForm() {
    if (degree == 0) {
        cout << "P(x) = " << an << endl;
        return;
    }

    cout << "P(x) = ";
    bool first = true;

    for (int i = degree; i >= 0; i--) {
        number coeff = coefficients->getElement(i);

        if (coeff.getRe() == 0 && coeff.getIm() == 0) {
            continue;
        }

        if (!first) {
            if (coeff.getRe() > 0 || (coeff.getRe() == 0 && coeff.getIm() > 0)) {
                cout << " + ";
            } else {
                cout << " ";
            }
        }

        if (i == 0) {
            cout << coeff;
        } else if (i == 1) {
            if (coeff.getRe() == 1 && coeff.getIm() == 0) {
                cout << "x";
            } else if (coeff.getRe() == -1 && coeff.getIm() == 0) {
                cout << "-x";
            } else {
                cout << "(" << coeff << ")*x";
            }
        } else {
            if (coeff.getRe() == 1 && coeff.getIm() == 0) {
                cout << "x^" << i;
            } else if (coeff.getRe() == -1 && coeff.getIm() == 0) {
                cout << "-x^" << i;
            } else {
                cout << "(" << coeff << ")*x^" << i;
            }
        }

        first = false;
    }

    if (first) {
        cout << "0";
    }
    cout << endl;
}

void Polynom::calculateCoefficients() {
    if (degree == 0) {
        coefficients->changeElem(0, an);
        return;
    }

    for (int i = 0; i <= degree; i++) {
        coefficients->changeElem(i, number(0, 0));
    }

    Array* temp = new Array();
    temp->create(degree + 1);
    temp->changeElem(0, number(1, 0));
    for (int i = 1; i <= degree; i++) {
        temp->changeElem(i, number(0, 0));
    }

    for (int rootIndex = 0; rootIndex < degree; rootIndex++) {
        number root = roots->getElement(rootIndex);

        Array* newTemp = new Array();
        newTemp->create(degree + 1);
        for (int i = 0; i <= degree; i++) {
            newTemp->changeElem(i, number(0, 0));
        }

        for (int i = 0; i <= degree; i++) {
            number currentCoeff = temp->getElement(i);
            if (currentCoeff.getRe() != 0 || currentCoeff.getIm() != 0) {
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

int Polynom::getDegree() const {
    return degree;
}

Array* Polynom::getRoots() const {
    return roots;
}

Array* Polynom::getCoefficients() const {
    return coefficients;
}

number Polynom::getLeadingCoefficient() const {
    return an;
}

