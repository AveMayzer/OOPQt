#ifndef SERVER_APPLICATION_H
#define SERVER_APPLICATION_H

#include <QCoreApplication>
#include "../communicator/communicator.h"
#include "../communicator/common.h"
#include "polynom.h"

class ServerApplication : public QCoreApplication
{
    Q_OBJECT

    TCommunicator *comm;
    Polynom* polynomial;

public:
    ServerApplication(int argc, char** argv);
    ~ServerApplication();

public slots:
    void recieve(QByteArray msg);

private:
    QString processRequest(int messageType, const QStringList& params);
};

#endif
