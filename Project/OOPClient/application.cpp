#include "application.h"
#include <QDebug>

ClientApplication::ClientApplication(int argc, char *argv[])
    : QApplication(argc, argv)
{

    TCommParams pars = { QHostAddress("127.0.0.1"), 10000,
                        QHostAddress("127.0.0.1"), 10001 };
    comm = new TCommunicator(pars, this);

    interface = new TInterface();
    interface->show();


    connect(comm, SIGNAL(recieved(QByteArray)), this, SLOT(fromCommunicator(QByteArray)));
    connect(interface, SIGNAL(request(QString)), this, SLOT(toCommunicator(QString)));

    qDebug() << "Client application started with communicator";
}

void ClientApplication::fromCommunicator(QByteArray msg)
{
    qDebug() << "Calling interface->answer() with:" << QString(msg);
    interface->answer(QString(msg));
}

void ClientApplication::toCommunicator(QString msg)
{
    qDebug() << "Sending to server:" << msg;
    comm->send(QByteArray().append(msg.toUtf8()));
}
