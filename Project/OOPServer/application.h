#ifndef SERVER_APPLICATION_H
#define SERVER_APPLICATION_H

#include <QCoreApplication>
#include "complex.h"
#include "../communicator/communicator.h"
#include "../communicator/common.h"

class ServerApplication : public QCoreApplication
{
    Q_OBJECT

    TCommunicator *comm;
    PolynomMode currentMode;

    QString processRequest(int messageType, const QStringList& params);
    QString formatComplexNumber(const TComplex& num);
    QString formatRealNumber(const TComplex& num);

public:
    ServerApplication(int argc, char** argv);
    ~ServerApplication();

public slots:
    void recieve(QByteArray msg);
};

#endif
