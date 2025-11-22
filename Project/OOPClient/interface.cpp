#include "interface.h"
#include <QDebug>
#include <QApplication>

TInterface::TInterface(QWidget* parent) : QWidget(parent), polynomialCreated(false), isComplexMode(true)
{
    setupUI();
    setWindowTitle("Практическая работа №6");
    setFixedSize(1050, 900);
}

TInterface::~TInterface() = default;

void TInterface::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    modeGroup = new QGroupBox("Режим работы с полиномами");
    QHBoxLayout* modeLayout = new QHBoxLayout(modeGroup);

    realModeRadio = new QRadioButton("Вещественные числа");
    complexModeRadio = new QRadioButton("Комплексные числа");
    complexModeRadio->setChecked(true);

    realModeRadio->setStyleSheet("QRadioButton { font-size: 14px; font-weight: bold; color: #000000; }");
    complexModeRadio->setStyleSheet("QRadioButton { font-size: 14px; font-weight: bold; color: #000000; }");

    modeButtonGroup = new QButtonGroup(this);
    modeButtonGroup->addButton(realModeRadio, REAL_MODE);
    modeButtonGroup->addButton(complexModeRadio, COMPLEX_MODE);

    modeLayout->addWidget(realModeRadio);
    modeLayout->addWidget(complexModeRadio);
    modeLayout->addStretch();

    connect(realModeRadio, SIGNAL(clicked()), this, SLOT(onModeChanged()));
    connect(complexModeRadio, SIGNAL(clicked()), this, SLOT(onModeChanged()));

    modeGroup->setStyleSheet(
        "QGroupBox { "
        "   font-weight: bold; "
        "   font-size: 14px; "
        "   background-color: #e3f2fd; "
        "   border: 2px solid #1976d2; "
        "   border-radius: 5px; "
        "   padding: 15px; "
        "   margin-top: 10px; "
        "}"
        "QGroupBox::title { "
        "   subcontrol-origin: margin; "
        "   subcontrol-position: top left; "
        "   padding: 5px 10px; "
        "   color: #1976d2; "
        "}"
        );
    mainLayout->addWidget(modeGroup);

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

    leadingCoeffImLabel = new QLabel("(Im):");
    createLayout->addWidget(leadingCoeffImLabel, 1, 2);
    leadingCoeffIm = new QLineEdit("0");
    createLayout->addWidget(leadingCoeffIm, 1, 3);

    mainLayout->addWidget(createGroup);

    rootsGroup = new QGroupBox("Корни полинома");
    rootsLayout = new QVBoxLayout(rootsGroup);
    mainLayout->addWidget(rootsGroup);

    createPolynomBtn = new QPushButton("Создать полином");
    createPolynomBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 12px; font-size: 14px; }");
    connect(createPolynomBtn, SIGNAL(clicked()), this, SLOT(formRequest()));
    mainLayout->addWidget(createPolynomBtn);

    operationsGroup = new QGroupBox("Операции с полиномом");
    QGridLayout* operationsLayout = new QGridLayout(operationsGroup);

    operationsLayout->addWidget(new QLabel("Вычислить P(x), x = "), 0, 0);
    xValueRe = new QLineEdit("0");
    operationsLayout->addWidget(xValueRe, 0, 1);
    xValueImLabel = new QLabel("+ i*");
    operationsLayout->addWidget(xValueImLabel, 0, 2);
    xValueIm = new QLineEdit("0");
    operationsLayout->addWidget(xValueIm, 0, 3);
    calculateValueBtn = new QPushButton("Вычислить");
    connect(calculateValueBtn, SIGNAL(clicked()), this, SLOT(formRequest()));
    operationsLayout->addWidget(calculateValueBtn, 0, 4);

    operationsLayout->addWidget(new QLabel("Новый ведущий коэфф. (Re):"), 1, 0);
    newLeadingCoeffRe = new QLineEdit("1");
    operationsLayout->addWidget(newLeadingCoeffRe, 1, 1);
    newLeadingCoeffImLabel = new QLabel("(Im):");
    operationsLayout->addWidget(newLeadingCoeffImLabel, 1, 2);
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
    newRootImLabel = new QLabel("+ i*");
    operationsLayout->addWidget(newRootImLabel, 3, 3);
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

    polynomialDisplayLabel = new QLabel("Выберите режим работы и создайте полином");
    polynomialDisplayLabel->setStyleSheet("QLabel { padding: 15px; background-color: #ffffcc; border: 2px solid #ffaa00; font-size: 16px; font-family: 'Courier New', monospace; color: #cc6600; }");
    polynomialDisplayLabel->setWordWrap(true);
    polynomialDisplayLabel->setAlignment(Qt::AlignCenter);
    polynomialDisplayLabel->setMinimumHeight(60);
    mainLayout->addWidget(polynomialDisplayLabel);

    updateRootsGroup();
    updateOperationsEnabled();
    updateUIForMode();
}

void TInterface::onModeChanged()
{
    isComplexMode = (complexModeRadio->isChecked());

    qDebug() << "MODE CHANGED! isComplexMode =" << isComplexMode << "realModeRadio->isChecked() =" << realModeRadio->isChecked() << "complexModeRadio->isChecked() =" << complexModeRadio->isChecked();

    if (!isComplexMode) {
        leadingCoeffIm->setText("0");
        leadingCoeffIm->setReadOnly(true);
        leadingCoeffIm->setStyleSheet("QLineEdit { background-color: #f0f0f0; }");

        xValueIm->setText("0");
        xValueIm->setReadOnly(true);
        xValueIm->setStyleSheet("QLineEdit { background-color: #f0f0f0; }");

        newLeadingCoeffIm->setText("0");
        newLeadingCoeffIm->setReadOnly(true);
        newLeadingCoeffIm->setStyleSheet("QLineEdit { background-color: #f0f0f0; }");

        newRootIm->setText("0");
        newRootIm->setReadOnly(true);
        newRootIm->setStyleSheet("QLineEdit { background-color: #f0f0f0; }");

        for (int i = 0; i < rootImInputs.size(); i++) {
            rootImInputs[i]->setText("0");
            rootImInputs[i]->setReadOnly(true);
            rootImInputs[i]->setStyleSheet("QLineEdit { background-color: #f0f0f0; }");
        }
    }
    else {
        leadingCoeffIm->setReadOnly(false);
        leadingCoeffIm->setStyleSheet("");

        xValueIm->setReadOnly(false);
        xValueIm->setStyleSheet("");

        newLeadingCoeffIm->setReadOnly(false);
        newLeadingCoeffIm->setStyleSheet("");

        newRootIm->setReadOnly(false);
        newRootIm->setStyleSheet("");

        for (int i = 0; i < rootImInputs.size(); i++) {
            rootImInputs[i]->setReadOnly(false);
            rootImInputs[i]->setStyleSheet("");
        }
    }

    updateUIForMode();

    QString modeText = isComplexMode ? "комплексных чисел" : "вещественных чисел";
    updateDisplayText(QString("Режим изменён на работу с %1.").arg(modeText),
                      "QLabel { padding: 15px; background-color: #e3f2fd; border: 2px solid #1976d2; font-size: 16px; font-family: 'Courier New', monospace; color: #1565c0; }");
}

void TInterface::updateUIForMode()
{

    leadingCoeffImLabel->setVisible(isComplexMode);
    leadingCoeffIm->setVisible(isComplexMode);

    xValueImLabel->setVisible(isComplexMode);
    xValueIm->setVisible(isComplexMode);

    newLeadingCoeffImLabel->setVisible(isComplexMode);
    newLeadingCoeffIm->setVisible(isComplexMode);

    newRootImLabel->setVisible(isComplexMode);
    newRootIm->setVisible(isComplexMode);

    for (int i = 0; i < rootImInputs.size(); i++) {
        if (i < rootImLabels.size()) {
            rootImLabels[i]->setVisible(isComplexMode);
        }
        rootImInputs[i]->setVisible(isComplexMode);
    }
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
        rootLayout->addWidget(reInput);

        QLabel* imLabel = new QLabel("+ i*");
        imLabel->setVisible(isComplexMode);
        rootLayout->addWidget(imLabel);

        QLineEdit* imInput = new QLineEdit("0");
        imInput->setVisible(isComplexMode);

        if (!isComplexMode) {
            imInput->setReadOnly(true);
            imInput->setStyleSheet("QLineEdit { background-color: #f0f0f0; }");
        }

        rootLayout->addWidget(imInput);

        rootReInputs.append(reInput);
        rootImInputs.append(imInput);
        rootImLabels.append(imLabel);

        rootsLayout->addWidget(rootWidget);
    }
}

void TInterface::clearRootsInputs()
{
    rootReInputs.clear();
    rootImInputs.clear();
    rootImLabels.clear();

    rootsGroup->setUpdatesEnabled(false);

    while (rootsLayout->count() > 0) {
        QLayoutItem* item = rootsLayout->takeAt(0);
        if (item->widget()) {
            item->widget()->hide();
            item->widget()->deleteLater();
        }
        delete item;
    }

    rootsGroup->setUpdatesEnabled(true);
}
void TInterface::updateOperationsEnabled()
{
    bool enabled = polynomialCreated;
    operationsGroup->setEnabled(enabled);
    displayGroup->setEnabled(enabled);


    modeGroup->setEnabled(true);
}

void TInterface::updateDisplayText(const QString& text, const QString& style)
{
    QString finalStyle = style.isEmpty() ? "QLabel { padding: 15px; background-color: #ffffff; border: 2px solid #333333; font-size: 16px; font-family: 'Courier New', monospace; color: #000000; }" : style;

    polynomialDisplayLabel->setText(text);
    polynomialDisplayLabel->setStyleSheet(finalStyle);

    qDebug() << "Display updated. Current text:" << polynomialDisplayLabel->text();
}

QString TInterface::appendPolynomialData(QString msg)
{
    msg << QString().setNum(degreeSpinBox->value());
    msg << (leadingCoeffRe->text().isEmpty() ? "0" : leadingCoeffRe->text());
    msg << (isComplexMode ? (leadingCoeffIm->text().isEmpty() ? "0" : leadingCoeffIm->text()) : "0");

    for (int i = 0; i < rootReInputs.size(); i++) {
        msg << (rootReInputs[i]->text().isEmpty() ? "0" : rootReInputs[i]->text());
        msg << (isComplexMode ? (rootImInputs[i]->text().isEmpty() ? "0" : rootImInputs[i]->text()) : "0");
    }

    return msg;
}

void TInterface::formRequest()
{
    QString msg;
    QPushButton* btn = (QPushButton*)sender();

    msg << QString::number(isComplexMode ? COMPLEX_MODE : REAL_MODE);
    msg << ";";

    if (btn == createPolynomBtn) {
        updateDisplayText("Создание полинома.",
                          "QLabel { padding: 15px; background-color: #ffffcc; border: 2px solid #ffaa00; font-size: 16px; font-family: 'Courier New', monospace; color: #cc6600; }");

        msg << QString().setNum(CREATE_POLYNOMIAL_REQUEST);
        msg << QString().setNum(degreeSpinBox->value());
        msg << (leadingCoeffRe->text().isEmpty() ? "0" : leadingCoeffRe->text());

        if (isComplexMode) {
            msg << (leadingCoeffIm->text().isEmpty() ? "0" : leadingCoeffIm->text());
        }
        else {
            msg << "0";
        }

        for (int i = 0; i < rootReInputs.size(); i++) {
            msg << (rootReInputs[i]->text().isEmpty() ? "0" : rootReInputs[i]->text());

            if (isComplexMode) {
                msg << (rootImInputs[i]->text().isEmpty() ? "0" : rootImInputs[i]->text());
            }
            else {
                msg << "0";
            }
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

            msg = appendPolynomialData(msg);

            msg << (xValueRe->text().isEmpty() ? "0" : xValueRe->text());
            msg << (isComplexMode ? (xValueIm->text().isEmpty() ? "0" : xValueIm->text()) : "0");
        }
        else if (btn == changeLeadingCoeffBtn) {
            msg << QString().setNum(CHANGE_LEADING_COEFF_REQUEST);

            msg = appendPolynomialData(msg);

            msg << (newLeadingCoeffRe->text().isEmpty() ? "0" : newLeadingCoeffRe->text());
            msg << (isComplexMode ? (newLeadingCoeffIm->text().isEmpty() ? "0" : newLeadingCoeffIm->text()) : "0");
        }
        else if (btn == resizeBtn) {
            msg << QString().setNum(RESIZE_POLYNOMIAL_REQUEST);

            msg = appendPolynomialData(msg);

            msg << QString().setNum(newDegreeSpinBox->value());
        }
        else if (btn == changeRootBtn) {
            msg << QString().setNum(CHANGE_ROOT_REQUEST);

            msg = appendPolynomialData(msg);

            msg << QString().setNum(rootIndexSpinBox->value() - 1);
            msg << (newRootRe->text().isEmpty() ? "0" : newRootRe->text());
            msg << (isComplexMode ? (newRootIm->text().isEmpty() ? "0" : newRootIm->text()) : "0");
        }
        else if (btn == showFactorFormBtn) {
            updateDisplayText("Получение факторной формы.",
                              "QLabel { padding: 15px; background-color: #ffffcc; border: 2px solid #ffaa00; font-size: 16px; font-family: 'Courier New', monospace; color: #cc6600; }");
            msg << QString().setNum(SHOW_FACTOR_FORM_REQUEST);

            msg = appendPolynomialData(msg);
        }
        else if (btn == showStandardFormBtn) {
            updateDisplayText("Получение стандартной формы.",
                              "QLabel { padding: 15px; background-color: #ffffcc; border: 2px solid #ffaa00; font-size: 16px; font-family: 'Courier New', monospace; color: #cc6600; }");
            msg << QString().setNum(SHOW_STANDARD_FORM_REQUEST);

            msg = appendPolynomialData(msg);
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

    qDebug() << "Message type:" << type;

    switch (type) {
    case CREATE_POLYNOMIAL_ANSWER:
        polynomialCreated = true;
        updateOperationsEnabled();
        updateDisplayText("Полином создан! Теперь можно выполнять операции.",
                          "QLabel { padding: 15px; background-color: #e8f5e8; border: 2px solid #4CAF50; font-size: 16px; font-family: 'Courier New', monospace; color: #2e7d32; }");
        break;

    case CHANGE_LEADING_COEFF_ANSWER:
    case RESIZE_POLYNOMIAL_ANSWER:
    case CHANGE_ROOT_ANSWER: {
        QStringList parts = msg.split(separator, Qt::SkipEmptyParts);
        if (parts.size() >= 3) {
            int newDegree = parts[0].toInt();
            double newLeadingRe = parts[1].toDouble();
            double newLeadingIm = parts[2].toDouble();
            setUpdatesEnabled(false);
            degreeSpinBox->blockSignals(true);
            degreeSpinBox->setValue(newDegree);
            leadingCoeffRe->setText(QString::number(newLeadingRe));
            leadingCoeffIm->setText(QString::number(newLeadingIm));

            if (newDegree != rootReInputs.size()) {

                clearRootsInputs();
                int degree = newDegree;

                if (degree == 0) {
                    QLabel* noRootsLabel = new QLabel("Полином степени 0 не имеет корней");
                    rootsLayout->addWidget(noRootsLabel);
                } else {
                    for (int i = 0; i < degree; i++) {
                        QWidget* rootWidget = new QWidget();
                        QHBoxLayout* rootLayout = new QHBoxLayout(rootWidget);

                        rootLayout->addWidget(new QLabel(QString("Корень %1:").arg(i + 1)));

                        QLineEdit* reInput = new QLineEdit("0");
                        rootLayout->addWidget(reInput);

                        QLabel* imLabel = new QLabel("+ i*");
                        imLabel->setVisible(isComplexMode);
                        rootLayout->addWidget(imLabel);

                        QLineEdit* imInput = new QLineEdit("0");
                        imInput->setVisible(isComplexMode);

                        if (!isComplexMode) {
                            imInput->setReadOnly(true);
                            imInput->setStyleSheet("QLineEdit { background-color: #f0f0f0; }");
                        }

                        rootLayout->addWidget(imInput);

                        rootReInputs.append(reInput);
                        rootImInputs.append(imInput);
                        rootImLabels.append(imLabel);

                        rootsLayout->addWidget(rootWidget);
                    }
                }
            }

            int paramIndex = 3;
            for (int i = 0; i < newDegree && paramIndex + 1 < parts.size(); i++) {
                double rootRe = parts[paramIndex++].toDouble();
                double rootIm = parts[paramIndex++].toDouble();

                if (i < rootReInputs.size()) {
                    rootReInputs[i]->setText(QString::number(rootRe));
                    rootImInputs[i]->setText(QString::number(rootIm));
                }
            }

            rootIndexSpinBox->setRange(1, qMax(1, newDegree));

            degreeSpinBox->blockSignals(false);
            setUpdatesEnabled(true);
            QString operationText;
            if (type == CHANGE_LEADING_COEFF_ANSWER) {
                operationText = "Ведущий коэффициент изменён успешно.";
            } else if (type == RESIZE_POLYNOMIAL_ANSWER) {
                operationText = "Степень полинома изменена успешно.";
            } else {
                operationText = "Корень изменён успешно.";
            }

            updateDisplayText(operationText,
                              "QLabel { padding: 15px; background-color: #e8f5e8; border: 2px solid #4CAF50; font-size: 16px; font-family: 'Courier New', monospace; color: #2e7d32; }");
        }
        break;
    }

    case CALCULATE_VALUE_ANSWER:
    case SHOW_POLYNOMIAL_ANSWER: {
        pos = msg.indexOf(separator);
        QString result = (pos == -1) ? msg : msg.left(pos);
        updateDisplayText(result,
                          "QLabel { padding: 15px; background-color: #f0f8ff; border: 2px solid #1976d2; font-size: 16px; font-family: 'Courier New', monospace; color: #1565c0; }");
        break;
    }

    case ERROR_ANSWER: {
        pos = msg.indexOf(separator);
        QString result = (pos == -1) ? msg : msg.left(pos);
        updateDisplayText("Ошибка: " + result,
                          "QLabel { padding: 15px; background-color: #ffe8e8; border: 2px solid #ff0000; font-size: 16px; font-family: 'Courier New', monospace; color: #cc0000; }");
        break;
    }

    default:
        qDebug() << "Unknown message type:" << type;
        updateDisplayText("Неизвестный тип ответа: " + QString::number(type),
                          "QLabel { padding: 15px; background-color: #ffe8e8; border: 2px solid #ff0000; font-size: 16px; font-family: 'Courier New', monospace; color: #cc0000; }");
        break;
    }
}
