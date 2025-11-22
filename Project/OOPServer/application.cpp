#include "application.h"
#include "polynom.h"
#include <QDebug>
#include <sstream>
#include <iostream>

ServerApplication::ServerApplication(int argc, char *argv[]) : QCoreApplication(argc, argv)
{
    TCommParams pars = { QHostAddress("127.0.0.1"), 10001,
                        QHostAddress("127.0.0.1"), 10000 };
    comm = new TCommunicator(pars, this);
    currentMode = COMPLEX_MODE;

    connect(comm, SIGNAL(recieved(QByteArray)), this, SLOT(recieve(QByteArray)));

    qDebug() << "Polynomial Server started on port 10001";
}

ServerApplication::~ServerApplication()
{
}

void ServerApplication::recieve(QByteArray msg)
{
    QString message = QString::fromUtf8(msg);
    QStringList parts = message.split(separator, Qt::SkipEmptyParts);

    if (parts.isEmpty()) {
        QString response;
        response << QString().setNum(ERROR_ANSWER) << "Пустое сообщение";
        comm->send(QByteArray().append(response.toLatin1()));
        return;
    }

    // Первый параметр - режим работы (0 = вещественный, 1 = комплексный)
    int mode = parts[0].toInt();
    currentMode = static_cast<PolynomMode>(mode);

    int messageType = parts[1].toInt();
    QStringList params = parts.mid(2);

    QString response = processRequest(messageType, params);
    comm->send(QByteArray().append(response.toLatin1()));
}

// Форматирование числа как комплексного
QString ServerApplication::formatComplexNumber(const TComplex& num)
{
    if (num.getIm() == 0) {
        return QString::number(num.getRe());
    }
    if (num.getRe() == 0) {
        return QString("%1i").arg(num.getIm());
    }
    if (num.getIm() > 0) {
        return QString("%1+%2i").arg(num.getRe()).arg(num.getIm());
    } else {
        return QString("%1%2i").arg(num.getRe()).arg(num.getIm());
    }
}

// Форматирование числа как вещественного (игнорируем мнимую часть)
QString ServerApplication::formatRealNumber(const TComplex& num)
{
    return QString::number(num.getRe());
}

QString ServerApplication::processRequest(int messageType, const QStringList& params)
{
    QString response;

    try {
        switch (messageType) {
        case CREATE_POLYNOMIAL_REQUEST: {
            if (params.size() < 3) {
                response << QString().setNum(ERROR_ANSWER) << "Недостаточно параметров";
                break;
            }

            int degree = params[0].toInt();
            double leadingRe = params[1].toDouble();
            double leadingIm = params[2].toDouble();

            Polynom<TComplex> polynomial(degree, TComplex(leadingRe, leadingIm));

            int paramIndex = 3;
            for (int i = 0; i < degree && paramIndex + 1 < params.size(); i++) {
                double rootRe = params[paramIndex++].toDouble();
                double rootIm = params[paramIndex++].toDouble();
                polynomial.changeRoot(i, TComplex(rootRe, rootIm));
            }

            QString modeText = (currentMode == COMPLEX_MODE) ? "Комплексный" : "Вещественный";
            response << QString().setNum(CREATE_POLYNOMIAL_ANSWER)
                     << QString("%1 полином степени %2 создан успешно").arg(modeText).arg(degree);
            break;
        }

        case CALCULATE_VALUE_REQUEST: {
            if (params.size() < 5) {
                response << QString().setNum(ERROR_ANSWER) << "Недостаточно параметров";
                break;
            }

            int degree = params[0].toInt();
            double leadingRe = params[1].toDouble();
            double leadingIm = params[2].toDouble();

            Polynom<TComplex> polynomial(degree, TComplex(leadingRe, leadingIm));

            int paramIndex = 3;
            for (int i = 0; i < degree && paramIndex + 1 < params.size() - 2; i++) {
                double rootRe = params[paramIndex++].toDouble();
                double rootIm = params[paramIndex++].toDouble();
                polynomial.changeRoot(i, TComplex(rootRe, rootIm));
            }

            double xRe = params[paramIndex++].toDouble();
            double xIm = params[paramIndex++].toDouble();
            TComplex x(xRe, xIm);
            TComplex result = polynomial.calculateValue(x);

            QString resultStr;
            if (currentMode == COMPLEX_MODE) {
                resultStr = QString("P(%1) = %2")
                .arg(formatComplexNumber(x))
                    .arg(formatComplexNumber(result));
            } else {
                resultStr = QString("P(%1) = %2")
                .arg(formatRealNumber(x))
                    .arg(formatRealNumber(result));
            }

            response << QString().setNum(CALCULATE_VALUE_ANSWER) << resultStr;
            break;
        }

        case CHANGE_LEADING_COEFF_REQUEST: {
            if (params.size() < 5) {
                response << QString().setNum(ERROR_ANSWER) << "Недостаточно параметров";
                break;
            }

            int degree = params[0].toInt();
            double oldLeadingRe = params[1].toDouble();
            double oldLeadingIm = params[2].toDouble();

            Polynom<TComplex> polynomial(degree, TComplex(oldLeadingRe, oldLeadingIm));

            int paramIndex = 3;
            for (int i = 0; i < degree && paramIndex + 1 < params.size() - 2; i++) {
                double rootRe = params[paramIndex++].toDouble();
                double rootIm = params[paramIndex++].toDouble();
                polynomial.changeRoot(i, TComplex(rootRe, rootIm));
            }

            double newRe = params[paramIndex++].toDouble();
            double newIm = params[paramIndex++].toDouble();
            polynomial.changeLeadingCoefficient(TComplex(newRe, newIm));

            // Возвращаем обновлённые данные полинома
            response << QString().setNum(CHANGE_LEADING_COEFF_ANSWER)
                     << QString().setNum(degree)
                     << QString().setNum(newRe)
                     << QString().setNum(newIm);

            Array<TComplex>* roots = polynomial.getRoots();
            for (int i = 0; i < degree; i++) {
                TComplex root = roots->getElement(i);
                response << QString().setNum(root.getRe());
                response << QString().setNum(root.getIm());
            }
            break;
        }

        case RESIZE_POLYNOMIAL_REQUEST: {
            if (params.size() < 4) {
                response << QString().setNum(ERROR_ANSWER) << "Недостаточно параметров";
                break;
            }

            int degree = params[0].toInt();
            double leadingRe = params[1].toDouble();
            double leadingIm = params[2].toDouble();

            Polynom<TComplex> polynomial(degree, TComplex(leadingRe, leadingIm));

            int paramIndex = 3;
            for (int i = 0; i < degree && paramIndex + 1 < params.size() - 1; i++) {
                double rootRe = params[paramIndex++].toDouble();
                double rootIm = params[paramIndex++].toDouble();
                polynomial.changeRoot(i, TComplex(rootRe, rootIm));
            }

            int newDegree = params[paramIndex].toInt();
            polynomial.resize(newDegree);

            // Возвращаем обновлённые данные полинома
            response << QString().setNum(RESIZE_POLYNOMIAL_ANSWER)
                     << QString().setNum(newDegree)
                     << QString().setNum(polynomial.getLeadingCoefficient().getRe())
                     << QString().setNum(polynomial.getLeadingCoefficient().getIm());

            Array<TComplex>* roots = polynomial.getRoots();
            for (int i = 0; i < newDegree; i++) {
                TComplex root = roots->getElement(i);
                response << QString().setNum(root.getRe());
                response << QString().setNum(root.getIm());
            }
            break;
        }

        case CHANGE_ROOT_REQUEST: {
            if (params.size() < 6) {
                response << QString().setNum(ERROR_ANSWER) << "Недостаточно параметров";
                break;
            }

            int degree = params[0].toInt();
            double leadingRe = params[1].toDouble();
            double leadingIm = params[2].toDouble();

            Polynom<TComplex> polynomial(degree, TComplex(leadingRe, leadingIm));

            int paramIndex = 3;
            for (int i = 0; i < degree && paramIndex + 1 < params.size() - 3; i++) {
                double rootRe = params[paramIndex++].toDouble();
                double rootIm = params[paramIndex++].toDouble();
                polynomial.changeRoot(i, TComplex(rootRe, rootIm));
            }

            int index = params[paramIndex++].toInt();
            double rootRe = params[paramIndex++].toDouble();
            double rootIm = params[paramIndex++].toDouble();
            polynomial.changeRoot(index, TComplex(rootRe, rootIm));

            // Возвращаем обновлённые данные полинома
            response << QString().setNum(CHANGE_ROOT_ANSWER)
                     << QString().setNum(degree)
                     << QString().setNum(polynomial.getLeadingCoefficient().getRe())
                     << QString().setNum(polynomial.getLeadingCoefficient().getIm());

            Array<TComplex>* roots = polynomial.getRoots();
            for (int i = 0; i < degree; i++) {
                TComplex root = roots->getElement(i);
                response << QString().setNum(root.getRe());
                response << QString().setNum(root.getIm());
            }
            break;
        }

        case SHOW_FACTOR_FORM_REQUEST: {
            if (params.size() < 3) {
                response << QString().setNum(ERROR_ANSWER) << "Недостаточно параметров";
                break;
            }

            int degree = params[0].toInt();
            double leadingRe = params[1].toDouble();
            double leadingIm = params[2].toDouble();

            Polynom<TComplex> polynomial(degree, TComplex(leadingRe, leadingIm));

            int paramIndex = 3;
            for (int i = 0; i < degree && paramIndex + 1 < params.size(); i++) {
                double rootRe = params[paramIndex++].toDouble();
                double rootIm = params[paramIndex++].toDouble();
                polynomial.changeRoot(i, TComplex(rootRe, rootIm));
            }

            QString output = "P(x) = ";

            TComplex leadingCoeff = polynomial.getLeadingCoefficient();

            // Показываем ведущий коэффициент
            if (currentMode == COMPLEX_MODE) {
                if (!(leadingCoeff.getRe() == 1 && leadingCoeff.getIm() == 0)) {
                    output += QString("(%1)").arg(formatComplexNumber(leadingCoeff));
                }
            } else {
                if (leadingCoeff.getRe() != 1) {
                    output += QString("(%1)").arg(formatRealNumber(leadingCoeff));
                }
            }

            // Показываем корни
            Array<TComplex>* roots = polynomial.getRoots();

            for (int i = 0; i < degree; i++) {
                TComplex root = roots->getElement(i);
                output += " * (x";

                if (currentMode == COMPLEX_MODE) {
                    if (!(root.getRe() == 0 && root.getIm() == 0)) {
                        output += QString(" - (%1)").arg(formatComplexNumber(root));
                    }
                } else {
                    if (root.getRe() != 0) {
                        output += QString(" - %1").arg(formatRealNumber(root));
                    }
                }
                output += ")";
            }

            response << QString().setNum(SHOW_POLYNOMIAL_ANSWER) << output;
            break;
        }

        case SHOW_STANDARD_FORM_REQUEST: {
            if (params.size() < 3) {
                response << QString().setNum(ERROR_ANSWER) << "Недостаточно параметров";
                break;
            }

            int degree = params[0].toInt();
            double leadingRe = params[1].toDouble();
            double leadingIm = params[2].toDouble();

            Polynom<TComplex> polynomial(degree, TComplex(leadingRe, leadingIm));

            int paramIndex = 3;
            for (int i = 0; i < degree && paramIndex + 1 < params.size(); i++) {
                double rootRe = params[paramIndex++].toDouble();
                double rootIm = params[paramIndex++].toDouble();
                polynomial.changeRoot(i, TComplex(rootRe, rootIm));
            }

            QString output = "P(x) = ";
            bool first = true;

            Array<TComplex>* coefficients = polynomial.getCoefficients();

            for (int i = degree; i >= 0; i--) {
                TComplex coeff = coefficients->getElement(i);

                bool isZero = (currentMode == COMPLEX_MODE)
                                  ? (coeff.getRe() == 0 && coeff.getIm() == 0)
                                  : (coeff.getRe() == 0);

                if (isZero) continue;

                if (!first) {
                    bool isPositive = (currentMode == COMPLEX_MODE)
                    ? (coeff.getRe() > 0 || (coeff.getRe() == 0 && coeff.getIm() > 0))
                    : (coeff.getRe() > 0);

                    if (isPositive) {
                        output += " + ";
                    } else {
                        output += " ";
                    }
                }

                QString coeffStr = (currentMode == COMPLEX_MODE)
                                       ? formatComplexNumber(coeff)
                                       : formatRealNumber(coeff);

                if (i == 0) {
                    output += coeffStr;
                } else if (i == 1) {
                    bool isOne = (currentMode == COMPLEX_MODE)
                    ? (coeff.getRe() == 1 && coeff.getIm() == 0)
                    : (coeff.getRe() == 1);
                    bool isMinusOne = (currentMode == COMPLEX_MODE)
                                          ? (coeff.getRe() == -1 && coeff.getIm() == 0)
                                          : (coeff.getRe() == -1);

                    if (isOne) {
                        output += "x";
                    } else if (isMinusOne) {
                        output += "-x";
                    } else {
                        output += QString("(%1)*x").arg(coeffStr);
                    }
                } else {
                    bool isOne = (currentMode == COMPLEX_MODE)
                    ? (coeff.getRe() == 1 && coeff.getIm() == 0)
                    : (coeff.getRe() == 1);
                    bool isMinusOne = (currentMode == COMPLEX_MODE)
                                          ? (coeff.getRe() == -1 && coeff.getIm() == 0)
                                          : (coeff.getRe() == -1);

                    if (isOne) {
                        output += QString("x^%1").arg(i);
                    } else if (isMinusOne) {
                        output += QString("-x^%1").arg(i);
                    } else {
                        output += QString("(%1)*x^%2").arg(coeffStr).arg(i);
                    }
                }

                first = false;
            }

            if (first) {
                output += "0";
            }

            response << QString().setNum(SHOW_POLYNOMIAL_ANSWER) << output;
            break;
        }

        default:
            response << QString().setNum(ERROR_ANSWER) << "Неизвестный тип сообщения";
        }
    }
    catch (const std::exception& e) {
        response = QString();
        response << QString().setNum(ERROR_ANSWER) << QString("Ошибка сервера: %1").arg(e.what());
    }

    return response;
}
