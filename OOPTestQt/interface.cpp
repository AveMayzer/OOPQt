#include "interface.h"
#include <sstream>

TInterface::TInterface(QWidget* parent) : QWidget(parent)
{
    polynomial = nullptr;
    polynomialCreated = false;
    setupUI();
    setWindowTitle("Практическая работа №4");
    setFixedSize(1000, 800);
}

TInterface::~TInterface()
{
    if (polynomial) {
        delete polynomial;
    }

}

void TInterface::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    createGroup = new QGroupBox("Параметры полинома", this);
    QGridLayout* createLayout = new QGridLayout(createGroup);

    createLayout->addWidget(new QLabel("Степень полинома:"), 0, 0);
    degreeSpinBox = new QSpinBox();
    degreeSpinBox->setRange(0, 10);
    degreeSpinBox->setValue(2);
    createLayout->addWidget(degreeSpinBox, 0, 1);

    createLayout->addWidget(new QLabel("Ведущий коэффициент (Re):"), 1, 0);
    leadingCoeffRe = new QLineEdit("1");
    createLayout->addWidget(leadingCoeffRe, 1, 1);

    createLayout->addWidget(new QLabel("Ведущий коэффициент (Im):"), 1, 2);
    leadingCoeffIm = new QLineEdit("0");
    createLayout->addWidget(leadingCoeffIm, 1, 3);

    connect(degreeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onDegreeChanged()));

    mainLayout->addWidget(createGroup);

    rootsGroup = new QGroupBox("Корни полинома", this);
    rootsLayout = new QVBoxLayout(rootsGroup);
    mainLayout->addWidget(rootsGroup);

    createPolynomBtn = new QPushButton("Создать полином");
    createPolynomBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 10px; }");
    connect(createPolynomBtn, SIGNAL(clicked()), this, SLOT(createPolynomial()));
    mainLayout->addWidget(createPolynomBtn);

    operationsGroup = new QGroupBox("Операции с полиномом", this);
    QGridLayout* operationsLayout = new QGridLayout(operationsGroup);

    operationsLayout->addWidget(new QLabel("Вычислить P(x), x = "), 0, 0);
    xValueRe = new QLineEdit("0");
    operationsLayout->addWidget(xValueRe, 0, 1);
    operationsLayout->addWidget(new QLabel("+ i*"), 0, 2);
    xValueIm = new QLineEdit("0");
    operationsLayout->addWidget(xValueIm, 0, 3);
    calculateValueBtn = new QPushButton("Вычислить");
    operationsLayout->addWidget(calculateValueBtn, 0, 4);

    operationsLayout->addWidget(new QLabel("Новый ведущий коэфф. (Re):"), 1, 0);
    newLeadingCoeffRe = new QLineEdit("1");
    operationsLayout->addWidget(newLeadingCoeffRe, 1, 1);
    operationsLayout->addWidget(new QLabel("(Im):"), 1, 2);
    newLeadingCoeffIm = new QLineEdit("0");
    operationsLayout->addWidget(newLeadingCoeffIm, 1, 3);
    changeLeadingCoeffBtn = new QPushButton("Изменить");
    operationsLayout->addWidget(changeLeadingCoeffBtn, 1, 4);

    operationsLayout->addWidget(new QLabel("Новая степень:"), 2, 0);
    newDegreeSpinBox = new QSpinBox();
    newDegreeSpinBox->setRange(0, 10);
    operationsLayout->addWidget(newDegreeSpinBox, 2, 1);
    resizeBtn = new QPushButton("Изменить размер");
    operationsLayout->addWidget(resizeBtn, 2, 4);

    operationsLayout->addWidget(new QLabel("Изменить корень, индекс:"), 3, 0);
    rootIndexSpinBox = new QSpinBox();
    rootIndexSpinBox->setRange(1, 10);
    rootIndexSpinBox->setValue(1);
    operationsLayout->addWidget(rootIndexSpinBox, 3, 1);

    newRootRe = new QLineEdit("0");
    operationsLayout->addWidget(newRootRe, 3, 2);
    operationsLayout->addWidget(new QLabel("+ i*"), 3, 3);
    newRootIm = new QLineEdit("0");
    operationsLayout->addWidget(newRootIm, 3, 4);

    changeRootBtn = new QPushButton("Изменить корень");
    operationsLayout->addWidget(changeRootBtn, 3, 5);

    connect(calculateValueBtn, SIGNAL(clicked()), this, SLOT(calculateValue()));
    connect(changeLeadingCoeffBtn, SIGNAL(clicked()), this, SLOT(changeLeadingCoeff()));
    connect(resizeBtn, SIGNAL(clicked()), this, SLOT(resizePolynomial()));
    connect(changeRootBtn, SIGNAL(clicked()), this, SLOT(changeRootByIndex()));

    mainLayout->addWidget(operationsGroup);

    displayGroup = new QGroupBox("Отображение полинома", this);
    QHBoxLayout* displayLayout = new QHBoxLayout(displayGroup);

    showFactorFormBtn = new QPushButton("Факторная форма");
    showStandardFormBtn = new QPushButton("Стандартная форма");

    displayLayout->addWidget(showFactorFormBtn);
    displayLayout->addWidget(showStandardFormBtn);

    connect(showFactorFormBtn, SIGNAL(clicked()), this, SLOT(showFactorForm()));
    connect(showStandardFormBtn, SIGNAL(clicked()), this, SLOT(showStandardForm()));

    mainLayout->addWidget(displayGroup);

    polynomialDisplayLabel = new QLabel("Создайте полином и нажмите кнопку для отображения");
    polynomialDisplayLabel->setStyleSheet("QLabel { padding: 15px; background-color: #ffffff; border: 2px solid #333333; font-size: 16px; font-family: 'Courier New', monospace; color: #000000; }");
    polynomialDisplayLabel->setWordWrap(true);
    polynomialDisplayLabel->setAlignment(Qt::AlignLeft);
    polynomialDisplayLabel->setMinimumHeight(60);
    mainLayout->addWidget(polynomialDisplayLabel);

    updateRootsGroup();
    updateOperationsEnabled();
}

void TInterface::onDegreeChanged()
{
    updateRootsGroup();
    polynomialCreated = false;
    updateOperationsEnabled();
    polynomialDisplayLabel->setText("Создайте полином и нажмите кнопку для отображения");

    int degree = degreeSpinBox->value();
    rootIndexSpinBox->setRange(1, qMax(1, degree));
}

void TInterface::updateRootsGroup()
{
    clearRootsInputs();
    int degree = degreeSpinBox->value();

    if (degree == 0) {
        QLabel* noRootsLabel = new QLabel("Полином степени 0 не имеет корней");
        rootsLayout->addWidget(noRootsLabel);
        return;
    }

    for (int i = 0; i < degree; i++) {
        QWidget* rootWidget = new QWidget();
        QHBoxLayout* rootLayout = new QHBoxLayout(rootWidget);

        rootLayout->addWidget(new QLabel(QString("Корень %1:").arg(i + 1)));

        QLineEdit* reInput = new QLineEdit("0");
        QLineEdit* imInput = new QLineEdit("0");

        rootLayout->addWidget(reInput);
        rootLayout->addWidget(new QLabel("+ i*"));
        rootLayout->addWidget(imInput);

        rootReInputs.append(reInput);
        rootImInputs.append(imInput);

        rootsLayout->addWidget(rootWidget);
    }
}

void TInterface::clearRootsInputs()
{
    rootReInputs.clear();
    rootImInputs.clear();
    while (rootsLayout->count() > 0) {
        QLayoutItem* item = rootsLayout->takeAt(0);
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

void TInterface::updateOperationsEnabled()
{
    bool enabled = polynomialCreated;
    operationsGroup->setEnabled(enabled);
    displayGroup->setEnabled(enabled);
}

void TInterface::createPolynomial()
{
    int degree = degreeSpinBox->value();
    number leadingCoeff = parseComplexNumber(leadingCoeffRe, leadingCoeffIm);

    if (polynomial) {
        delete polynomial;
    }

    polynomial = new Polynom(degree, leadingCoeff);

    if (degree > 0) {
        for (int i = 0; i < rootReInputs.size() && i < degree; i++) {
            number root = parseComplexNumber(rootReInputs[i], rootImInputs[i]);
            polynomial->changeRoot(i, root);
        }
    }

    polynomialCreated = true;
    updateOperationsEnabled();

    polynomialDisplayLabel->setText("Полином создан успешно. Нажмите кнопку для отображения.");
}

void TInterface::calculateValue()
{
    number x = parseComplexNumber(xValueRe, xValueIm);
    number result = polynomial->calculateValue(x);
    QString resultText = QString("Результат: P(%1 + %2i) = %3 + %4i")
        .arg(x.getRe()).arg(x.getIm())
        .arg(result.getRe()).arg(result.getIm());

    polynomialDisplayLabel->setText(resultText);
}

void TInterface::changeLeadingCoeff()
{
    number newCoeff = parseComplexNumber(newLeadingCoeffRe, newLeadingCoeffIm);
    polynomial->changeLeadingCoefficient(newCoeff);
    polynomialDisplayLabel->setText("Ведущий коэффициент изменен. Нажмите кнопку для отображения.");
}

void TInterface::resizePolynomial()
{
    int newDegree = newDegreeSpinBox->value();
    polynomial->resize(newDegree);
    polynomialDisplayLabel->setText("Размер полинома изменен. Нажмите кнопку для отображения.");
}

void TInterface::changeRootByIndex()
{
    int rootIndex = rootIndexSpinBox->value() - 1;
    number newRoot = parseComplexNumber(newRootRe, newRootIm);
    polynomial->changeRoot(rootIndex, newRoot);

    polynomialDisplayLabel->setText(QString("Корень %1 изменен. Нажмите кнопку для отображения.")
        .arg(rootIndex + 1));
}


void TInterface::showFactorForm()
{
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    polynomial->printFactorForm();

    std::cout.rdbuf(old_cout);
    QString output = QString::fromStdString(oss.str());
    polynomialDisplayLabel->setText(output.trimmed());
}

void TInterface::showStandardForm()
{
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    polynomial->printStandardForm();

    std::cout.rdbuf(old_cout);
    QString output = QString::fromStdString(oss.str());
    polynomialDisplayLabel->setText(output.trimmed());
}

number TInterface::parseComplexNumber(QLineEdit* reInput, QLineEdit* imInput)
{
    double re = reInput->text().toDouble();
    double im = imInput->text().toDouble();
    return number(re, im);
}
