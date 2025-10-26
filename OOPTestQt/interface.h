#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include "Polynom.h"

class TInterface : public QWidget
{
    Q_OBJECT

private:

    QVBoxLayout* mainLayout;

    QGroupBox* createGroup;
    QSpinBox* degreeSpinBox;
    QLineEdit* leadingCoeffRe;
    QLineEdit* leadingCoeffIm;

    QGroupBox* rootsGroup;
    QVBoxLayout* rootsLayout;
    QVector<QLineEdit*> rootReInputs;
    QVector<QLineEdit*> rootImInputs;

    QPushButton* createPolynomBtn;

    QGroupBox* operationsGroup;
    QLineEdit* xValueRe;
    QLineEdit* xValueIm;
    QPushButton* calculateValueBtn;
    QLineEdit* newLeadingCoeffRe;
    QLineEdit* newLeadingCoeffIm;
    QPushButton* changeLeadingCoeffBtn;
    QSpinBox* newDegreeSpinBox;
    QPushButton* resizeBtn;

    QSpinBox* rootIndexSpinBox;
    QLineEdit* newRootRe;
    QLineEdit* newRootIm;
    QPushButton* changeRootBtn;

    QGroupBox* displayGroup;
    QPushButton* showFactorFormBtn;
    QPushButton* showStandardFormBtn;

    QLabel* polynomialDisplayLabel;

    Polynom* polynomial;
    bool polynomialCreated;

public:
    TInterface(QWidget* parent = nullptr);
    ~TInterface();

private slots:
    void onDegreeChanged();
    void createPolynomial();
    void calculateValue();
    void changeLeadingCoeff();
    void resizePolynomial();
    void changeRootByIndex();
    void showFactorForm();
    void showStandardForm();

private:
    void setupUI();
    void updateRootsGroup();
    void updateOperationsEnabled();
    number parseComplexNumber(QLineEdit* reInput, QLineEdit* imInput);
    void clearRootsInputs();
};

#endif
