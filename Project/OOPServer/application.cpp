#include "application.h"
#include <QDebug>
#include <sstream>
#include <iostream>

ServerApplication::ServerApplication(int argc, char *argv[]) : QCoreApplication(argc, argv)
{

    TCommParams pars = { QHostAddress("127.0.0.1"), 10001,
                        QHostAddress("127.0.0.1"), 10000 };
    comm = new TCommunicator(pars, this);
    polynomial = nullptr;

    connect(comm, SIGNAL(recieved(QByteArray)), this, SLOT(recieve(QByteArray)));

    qDebug() << "Polynomial Server started on port 10001";
}

ServerApplication::~ServerApplication()
{
    if (polynomial) delete polynomial;
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

    int messageType = parts[0].toInt();
    QStringList params = parts.mid(1);

    QString response = processRequest(messageType, params);
    comm->send(QByteArray().append(response.toLatin1()));
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

            if (polynomial) delete polynomial;
            polynomial = new Polynom(degree, number(leadingRe, leadingIm));

            int paramIndex = 3;
            for (int i = 0; i < degree && paramIndex + 1 < params.size(); i++) {
                double rootRe = params[paramIndex++].toDouble();
                double rootIm = params[paramIndex++].toDouble();
                polynomial->changeRoot(i, number(rootRe, rootIm));
            }

            response << QString().setNum(CREATE_POLYNOMIAL_ANSWER)
                     << QString("Полином степени %1 создан успешно").arg(degree);
            break;
        }

        case CALCULATE_VALUE_REQUEST: {
            if (!polynomial) {
                response << QString().setNum(ERROR_ANSWER) << "Полином не создан";
                break;
            }
            if (params.size() < 2) {
                response << QString().setNum(ERROR_ANSWER) << "Недостаточно параметров";
                break;
            }

            double xRe = params[0].toDouble();
            double xIm = params[1].toDouble();
            number x(xRe, xIm);
            number result = polynomial->calculateValue(x);

            QString resultStr = QString("P(%1 + %2i) = %3 + %4i")
                                    .arg(x.getRe()).arg(x.getIm())
                                    .arg(result.getRe()).arg(result.getIm());

            response << QString().setNum(CALCULATE_VALUE_ANSWER) << resultStr;
            break;
        }

        case CHANGE_LEADING_COEFF_REQUEST: {
            if (!polynomial) {
                response << QString().setNum(ERROR_ANSWER) << "Полином не создан";
                break;
            }
            if (params.size() < 2) {
                response << QString().setNum(ERROR_ANSWER) << "Недостаточно параметров";
                break;
            }

            double newRe = params[0].toDouble();
            double newIm = params[1].toDouble();
            polynomial->changeLeadingCoefficient(number(newRe, newIm));

            response << QString().setNum(CHANGE_LEADING_COEFF_ANSWER)
                     << "Ведущий коэффициент изменен";
            break;
        }

        case RESIZE_POLYNOMIAL_REQUEST: {
            if (!polynomial) {
                response << QString().setNum(ERROR_ANSWER) << "Полином не создан";
                break;
            }
            if (params.isEmpty()) {
                response << QString().setNum(ERROR_ANSWER) << "Недостаточно параметров";
                break;
            }

            int newDegree = params[0].toInt();
            polynomial->resize(newDegree);

            response << QString().setNum(RESIZE_POLYNOMIAL_ANSWER)
                     << QString("Размер полинома изменен на %1").arg(newDegree);
            break;
        }

        case CHANGE_ROOT_REQUEST: {
            if (!polynomial) {
                response << QString().setNum(ERROR_ANSWER) << "Полinom не создан";
                break;
            }
            if (params.size() < 3) {
                response << QString().setNum(ERROR_ANSWER) << "Недостаточно параметров";
                break;
            }

            int index = params[0].toInt();
            double rootRe = params[1].toDouble();
            double rootIm = params[2].toDouble();
            polynomial->changeRoot(index, number(rootRe, rootIm));

            response << QString().setNum(CHANGE_ROOT_ANSWER)
                     << QString("Корень %1 изменен").arg(index + 1);
            break;
        }

        case SHOW_FACTOR_FORM_REQUEST: {
            if (!polynomial) {
                response << QString().setNum(ERROR_ANSWER) << "Полином не создан";
                break;
            }

            std::ostringstream oss;
            std::streambuf* old_cout = std::cout.rdbuf();
            std::cout.rdbuf(oss.rdbuf());

            polynomial->printFactorForm();

            std::cout.rdbuf(old_cout);
            QString output = QString::fromStdString(oss.str()).trimmed();

            response << QString().setNum(SHOW_POLYNOMIAL_ANSWER) << output;
            break;
        }

        case SHOW_STANDARD_FORM_REQUEST: {
            if (!polynomial) {
                response << QString().setNum(ERROR_ANSWER) << "Полином не создан";
                break;
            }

            std::ostringstream oss;
            std::streambuf* old_cout = std::cout.rdbuf();
            std::cout.rdbuf(oss.rdbuf());

            polynomial->printStandardForm();

            std::cout.rdbuf(old_cout);
            QString output = QString::fromStdString(oss.str()).trimmed();

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
