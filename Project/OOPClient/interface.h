#ifndef TINTERFACE_H
#define TINTERFACE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QVector>
#include "../communicator/common.h"

class TInterface : public QWidget
{
    Q_OBJECT

public:
    TInterface(QWidget *parent = nullptr);
    ~TInterface();

signals:
    void request(QString msg);

public slots:
    void answer(QString msg);

private slots:
    void formRequest();
    void onDegreeChanged();

private:
    void setupUI();
    void updateRootsGroup();
    void clearRootsInputs();
    void updateOperationsEnabled();
    void updateDisplayText(const QString& text, const QString& style = QString()); // ДОБАВЛЕНО


    QVBoxLayout* mainLayout;

    QGroupBox* createGroup;
    QSpinBox* degreeSpinBox;
    QLineEdit* leadingCoeffRe;
    QLineEdit* leadingCoeffIm;
    QPushButton* createPolynomBtn;

    QGroupBox* rootsGroup;
    QVBoxLayout* rootsLayout;
    QVector<QLineEdit*> rootReInputs;
    QVector<QLineEdit*> rootImInputs;

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

    bool polynomialCreated;
};

#endif // TINTERFACE_H
