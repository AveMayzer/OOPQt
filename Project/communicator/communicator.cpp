#include "communicator.h"

TCommunicator::TCommunicator(TCommParams& pars, QObject *parent) : QUdpSocket(parent) {
    params = pars;
    ready = bind(params.rHost, params.rPort,
                 QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
    if (ready) {
        connect(this, SIGNAL(readyRead()), this, SLOT(recieve()));
        qDebug() << "Communicator ready on port" << params.rPort;
    } else {
        qDebug() << "Failed to bind communicator on port" << params.rPort;
    }
}

void TCommunicator::send(QByteArray msg) {
    if (ready) {
        writeDatagram(msg, params.sHost, params.sPort);
        qDebug() << "Sent:" << msg;
    }
}

void TCommunicator::recieve() {
    if (hasPendingDatagrams()) {
        quint64 size = pendingDatagramSize();
        QByteArray msg(size, '\0');
        readDatagram(msg.data(), size);
        qDebug() << "Received:" << msg;
        emit recieved(msg);
    }
}
