#include "interface.h"
#include <QDebug>
#include <QApplication>

TInterface::TInterface(QWidget *parent) : QWidget(parent), polynomialCreated(false)
{
    setupUI();
    setWindowTitle("Клиент полиномов - Практическая работа №5");
    setFixedSize(1000, 800);
}

TInterface::~TInterface() = default;

void TInterface::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    createGroup = new QGroupBox("Параметры полинома");
    QGridLayout* createLayout = new QGridLayout(createGroup);

    createLayout->addWidget(new QLabel("Степень полинома:"), 0, 0);
    degreeSpinBox = new QSpinBox();
    degreeSpinBox->setRange(0, 10);
    degreeSpinBox->setValue(2);
    connect(degreeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onDegreeChanged()));
    createLayout->addWidget(degreeSpinBox, 0, 1);

    createLayout->addWidget(new QLabel("Ведущий коэффициент (Re):"), 1, 0);
    leadingCoeffRe = new QLineEdit("1");
    createLayout->addWidget(leadingCoeffRe, 1, 1);

    createLayout->addWidget(new QLabel("Ведущий коэффициент (Im):"), 1, 2);
    leadingCoeffIm = new QLineEdit("0");
    createLayout->addWidget(leadingCoeffIm, 1, 3);

    mainLayout->addWidget(createGroup);

    rootsGroup = new QGroupBox("Корни полинома");
    rootsLayout = new QVBoxLayout(rootsGroup);
    mainLayout->addWidget(rootsGroup);

    createPolynomBtn = new QPushButton("Создать полином");
    createPolynomBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 10px; }");
    connect(createPolynomBtn, SIGNAL(clicked()), this, SLOT(formRequest()));
    mainLayout->addWidget(createPolynomBtn);

    operationsGroup = new QGroupBox("Операции с полиномом");
    QGridLayout* operationsLayout = new QGridLayout(operationsGroup);

    operationsLayout->addWidget(new QLabel("Вычислить P(x), x = "), 0, 0);
    xValueRe = new QLineEdit("0");
    operationsLayout->addWidget(xValueRe, 0, 1);
    operationsLayout->addWidget(new QLabel("+ i*"), 0, 2);
    xValueIm = new QLineEdit("0");
    operationsLayout->addWidget(xValueIm, 0, 3);
    calculateValueBtn = new QPushButton("Вычислить");
    connect(calculateValueBtn, SIGNAL(clicked()), this, SLOT(formRequest()));
    operationsLayout->addWidget(calculateValueBtn, 0, 4);

    operationsLayout->addWidget(new QLabel("Новый ведущий коэфф. (Re):"), 1, 0);
    newLeadingCoeffRe = new QLineEdit("1");
    operationsLayout->addWidget(newLeadingCoeffRe, 1, 1);
    operationsLayout->addWidget(new QLabel("(Im):"), 1, 2);
    newLeadingCoeffIm = new QLineEdit("0");
    operationsLayout->addWidget(newLeadingCoeffIm, 1, 3);
    changeLeadingCoeffBtn = new QPushButton("Изменить");
    connect(changeLeadingCoeffBtn, SIGNAL(clicked()), this, SLOT(formRequest()));
    operationsLayout->addWidget(changeLeadingCoeffBtn, 1, 4);

    operationsLayout->addWidget(new QLabel("Новая степень:"), 2, 0);
    newDegreeSpinBox = new QSpinBox();
    newDegreeSpinBox->setRange(0, 10);
    operationsLayout->addWidget(newDegreeSpinBox, 2, 1);
    resizeBtn = new QPushButton("Изменить размер");
    connect(resizeBtn, SIGNAL(clicked()), this, SLOT(formRequest()));
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
    connect(changeRootBtn, SIGNAL(clicked()), this, SLOT(formRequest()));
    operationsLayout->addWidget(changeRootBtn, 3, 5);

    mainLayout->addWidget(operationsGroup);

    displayGroup = new QGroupBox("Отображение полинома");
    QHBoxLayout* displayLayout = new QHBoxLayout(displayGroup);

    showFactorFormBtn = new QPushButton("Факторная форма");
    connect(showFactorFormBtn, SIGNAL(clicked()), this, SLOT(formRequest()));
    showStandardFormBtn = new QPushButton("Стандартная форма");
    connect(showStandardFormBtn, SIGNAL(clicked()), this, SLOT(formRequest()));

    displayLayout->addWidget(showFactorFormBtn);
    displayLayout->addWidget(showStandardFormBtn);

    mainLayout->addWidget(displayGroup);

    polynomialDisplayLabel = new QLabel("Сначала создайте полином");
    polynomialDisplayLabel->setStyleSheet("QLabel { padding: 15px; background-color: #ffffcc; border: 2px solid #ffaa00; font-size: 16px; font-family: 'Courier New', monospace; color: #cc6600; }");
    polynomialDisplayLabel->setWordWrap(true);
    polynomialDisplayLabel->setAlignment(Qt::AlignCenter);
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

    updateDisplayText("Сначала создайте полином с выбранными параметрами");

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

void TInterface::updateDisplayText(const QString& text, const QString& style)
{
    QString finalStyle = style.isEmpty() ? "QLabel { padding: 15px; background-color: #ffffff; border: 2px solid #333333; font-size: 16px; font-family: 'Courier New', monospace; color: #000000; }" : style;

    polynomialDisplayLabel->setText(text);
    polynomialDisplayLabel->setStyleSheet(finalStyle);

    polynomialDisplayLabel->update();
    QApplication::processEvents();

    qDebug() << "Display updated. Current text:" << polynomialDisplayLabel->text();
}

void TInterface::formRequest()
{
    QString msg;
    QPushButton *btn = (QPushButton*)sender();


    if (btn == createPolynomBtn) {
        updateDisplayText("Создание полинома...",
                          "QLabel { padding: 15px; background-color: #ffffcc; border: 2px solid #ffaa00; font-size: 16px; font-family: 'Courier New', monospace; color: #cc6600; }");

        msg << QString().setNum(CREATE_POLYNOMIAL_REQUEST);
        msg << QString().setNum(degreeSpinBox->value());
        msg << (leadingCoeffRe->text().isEmpty() ? "0" : leadingCoeffRe->text());
        msg << (leadingCoeffIm->text().isEmpty() ? "0" : leadingCoeffIm->text());

        for (int i = 0; i < rootReInputs.size(); i++) {
            msg << (rootReInputs[i]->text().isEmpty() ? "0" : rootReInputs[i]->text());
            msg << (rootImInputs[i]->text().isEmpty() ? "0" : rootImInputs[i]->text());
        }
    }
    else {

        if (!polynomialCreated) {
            updateDisplayText("Ошибка: Сначала создайте полином!",
                              "QLabel { padding: 15px; background-color: #ffe8e8; border: 2px solid #ff0000; font-size: 16px; font-family: 'Courier New', monospace; color: #cc0000; }");
            return;
        }

        if (btn == calculateValueBtn) {
            updateDisplayText("Вычисление значения.",
                              "QLabel { padding: 15px; background-color: #ffffcc; border: 2px solid #ffaa00; font-size: 16px; font-family: 'Courier New', monospace; color: #cc6600; }");
            msg << QString().setNum(CALCULATE_VALUE_REQUEST);
            msg << (xValueRe->text().isEmpty() ? "0" : xValueRe->text());
            msg << (xValueIm->text().isEmpty() ? "0" : xValueIm->text());
        }
        else if (btn == changeLeadingCoeffBtn) {
            updateDisplayText("Изменение коэффициента.",
                              "QLabel { padding: 15px; background-color: #ffffcc; border: 2px solid #ffaa00; font-size: 16px; font-family: 'Courier New', monospace; color: #cc6600; }");
            msg << QString().setNum(CHANGE_LEADING_COEFF_REQUEST);
            msg << (newLeadingCoeffRe->text().isEmpty() ? "0" : newLeadingCoeffRe->text());
            msg << (newLeadingCoeffIm->text().isEmpty() ? "0" : newLeadingCoeffIm->text());
        }
        else if (btn == resizeBtn) {
            updateDisplayText("Изменение размера.",
                              "QLabel { padding: 15px; background-color: #ffffcc; border: 2px solid #ffaa00; font-size: 16px; font-family: 'Courier New', monospace; color: #cc6600; }");
            msg << QString().setNum(RESIZE_POLYNOMIAL_REQUEST);
            msg << QString().setNum(newDegreeSpinBox->value());
        }
        else if (btn == changeRootBtn) {
            updateDisplayText("Изменение корня.",
                              "QLabel { padding: 15px; background-color: #ffffcc; border: 2px solid #ffaa00; font-size: 16px; font-family: 'Courier New', monospace; color: #cc6600; }");
            msg << QString().setNum(CHANGE_ROOT_REQUEST);
            msg << QString().setNum(rootIndexSpinBox->value() - 1);
            msg << (newRootRe->text().isEmpty() ? "0" : newRootRe->text());
            msg << (newRootIm->text().isEmpty() ? "0" : newRootIm->text());
        }
        else if (btn == showFactorFormBtn) {
            updateDisplayText("Получение факторной формы...",
                              "QLabel { padding: 15px; background-color: #ffffcc; border: 2px solid #ffaa00; font-size: 16px; font-family: 'Courier New', monospace; color: #cc6600; }");
            msg << QString().setNum(SHOW_FACTOR_FORM_REQUEST);
        }
        else if (btn == showStandardFormBtn) {
            updateDisplayText("Получение стандартной формы...",
                              "QLabel { padding: 15px; background-color: #ffffcc; border: 2px solid #ffaa00; font-size: 16px; font-family: 'Courier New', monospace; color: #cc6600; }");
            msg << QString().setNum(SHOW_STANDARD_FORM_REQUEST);
        }
    }

    qDebug() << "Sending request:" << msg;
    emit request(msg);
}

void TInterface::answer(QString msg)
{

    if (msg.isEmpty()) {
        updateDisplayText("Ошибка: Пустой ответ от сервера",
                          "QLabel { padding: 15px; background-color: #ffe8e8; border: 2px solid #ff0000; font-size: 16px; font-family: 'Courier New', monospace; color: #cc0000; }");
        return;
    }

    const QChar separator(';');
    int pos = msg.indexOf(separator);
    if (pos == -1) {
        updateDisplayText("Ошибка: Неверный формат ответа",
                          "QLabel { padding: 15px; background-color: #ffe8e8; border: 2px solid #ff0000; font-size: 16px; font-family: 'Courier New', monospace; color: #cc0000; }");
        return;
    }

    int type = msg.left(pos).toInt();
    msg.remove(0, pos + 1);

    pos = msg.indexOf(separator);
    QString result = (pos == -1) ? msg : msg.left(pos);

    qDebug() << "Message type:" << type;

    switch (type) {
    case CREATE_POLYNOMIAL_ANSWER:
        polynomialCreated = true;
        updateOperationsEnabled();
        updateDisplayText("Теперь можно выполнять операции с полиномом.",
                          "QLabel { padding: 15px; background-color: #e8f5e8; border: 2px solid #4CAF50; font-size: 16px; font-family: 'Courier New', monospace; color: #2e7d32; }");
        break;

    case CHANGE_LEADING_COEFF_ANSWER:
        updateDisplayText("Ведущий коэффициент изменён.",
                          "QLabel { padding: 15px; background-color: #e8f5e8; border: 2px solid #4CAF50; font-size: 16px; font-family: 'Courier New', monospace; color: #2e7d32; }");
        break;
    case RESIZE_POLYNOMIAL_ANSWER:
        updateDisplayText("Новая степерь установлена",
                          "QLabel { padding: 15px; background-color: #e8f5e8; border: 2px solid #4CAF50; font-size: 16px; font-family: 'Courier New', monospace; color: #2e7d32; }");
        break;
    case CHANGE_ROOT_ANSWER:
        updateDisplayText("Корень изменён",
                          "QLabel { padding: 15px; background-color: #e8f5e8; border: 2px solid #4CAF50; font-size: 16px; font-family: 'Courier New', monospace; color: #2e7d32; }");
        break;

    case CALCULATE_VALUE_ANSWER:
    case SHOW_POLYNOMIAL_ANSWER:
        updateDisplayText(result,
                          "QLabel { padding: 15px; background-color: #f0f8ff; border: 2px solid #1976d2; font-size: 16px; font-family: 'Courier New', monospace; color: #1565c0; }");
        break;

    case ERROR_ANSWER:
        updateDisplayText("Ошибка: " + result,
                          "QLabel { padding: 15px; background-color: #ffe8e8; border: 2px solid #ff0000; font-size: 16px; font-family: 'Courier New', monospace; color: #cc0000; }");
        break;

    default:
        qDebug() << "Unknown message type:" << type;
        updateDisplayText("Неизвестный тип ответа: " + QString::number(type),
                          "QLabel { padding: 15px; background-color: #ffe8e8; border: 2px solid #ff0000; font-size: 16px; font-family: 'Courier New', monospace; color: #cc0000; }");
        break;
    }
}
