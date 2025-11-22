#ifndef TINTERFACE_H
#define TINTERFACE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>
#include <QList>
#include "../communicator/common.h"

class TInterface : public QWidget
{
    Q_OBJECT

    QVBoxLayout* mainLayout;
    QGroupBox* modeGroup;
    QGroupBox* createGroup;
    QGroupBox* rootsGroup;
    QGroupBox* operationsGroup;
    QGroupBox* displayGroup;

    QRadioButton* realModeRadio;
    QRadioButton* complexModeRadio;
    QButtonGroup* modeButtonGroup;

    QSpinBox* degreeSpinBox;
    QLineEdit* leadingCoeffRe;
    QLineEdit* leadingCoeffIm;
    QLabel* leadingCoeffImLabel;

    QVBoxLayout* rootsLayout;
    QList<QLineEdit*> rootReInputs;
    QList<QLineEdit*> rootImInputs;
    QList<QLabel*> rootImLabels;

    QPushButton* createPolynomBtn;

    QLineEdit* xValueRe;
    QLineEdit* xValueIm;
    QLabel* xValueImLabel;
    QPushButton* calculateValueBtn;

    QLineEdit* newLeadingCoeffRe;
    QLineEdit* newLeadingCoeffIm;
    QLabel* newLeadingCoeffImLabel;
    QPushButton* changeLeadingCoeffBtn;

    QSpinBox* newDegreeSpinBox;
    QPushButton* resizeBtn;

    QSpinBox* rootIndexSpinBox;
    QLineEdit* newRootRe;
    QLineEdit* newRootIm;
    QLabel* newRootImLabel;
    QPushButton* changeRootBtn;

    QPushButton* showFactorFormBtn;
    QPushButton* showStandardFormBtn;

    QLabel* polynomialDisplayLabel;

    bool polynomialCreated;
    bool isComplexMode;

    void setupUI();
    void updateRootsGroup();
    void clearRootsInputs();
    void updateOperationsEnabled();
    void updateUIForMode();
    void updateDisplayText(const QString& text, const QString& style = "");
    QString appendPolynomialData(QString msg);

public:
    explicit TInterface(QWidget* parent = nullptr);
    ~TInterface();

public slots:
    void formRequest();
    void answer(QString msg);
    void onModeChanged();
    void onDegreeChanged();

signals:
    void request(QString msg);
};

#endif
