#include "OscSender.h"
#include "contrib/oscpack/OscOutboundPacketStream.h"
#include "contrib/oscpack/OscTypes.h"
#include <iostream>
#include <chrono>
#include <QDebug>
#include <QThread>

// FIXME: we should also allow addresses such as "localhost", or "example.com"

OscSender::OscSender(const QString& hostAddress, quint16 port, QObject *parent) :
        QObject(parent),
        m_udpSocket(new QUdpSocket(this)),
        m_hostAddress(QHostAddress(hostAddress)),
        m_port(port)
{
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "constructor (OscSender):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    m_udpSocket->connectToHost(m_hostAddress , m_port);
}

OscSender::~OscSender()
{
    if(flagDebug) {
        std::chrono::nanoseconds now = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
        );

        qDebug() << "destructor (OscSender):\tt = " << now.count() << "\tid = " << QThread::currentThreadId();
    }

    m_udpSocket->close();
    delete m_udpSocket;
}

void OscSender::setPort(quint16 port)
{
    m_port = port;
    m_udpSocket->disconnectFromHost();
    m_udpSocket->connectToHost(m_hostAddress , m_port);
}

void OscSender::setOscSenderHost(const QString& host)
{
    m_hostAddress = QHostAddress(host);
    m_udpSocket->disconnectFromHost();
    m_udpSocket->connectToHost(m_hostAddress , m_port);
}

void OscSender::send(const QString& oscAddress, const QVariantList& arguments) {
    if(flagDebug) {
        qDebug() << "send (OscSender)\targuments = " << arguments;
    }

    QByteArray datagram;
    this->variantListToByteArray(datagram, oscAddress, arguments);

    qint64 written = m_udpSocket->write(datagram);

    if (flagDebug && written == -1) {
        qDebug() << "failed to send OSC (write bytes to the send socket)";
    }
    m_udpSocket->flush();
    m_udpSocket->waitForBytesWritten();
}


void OscSender::variantListToByteArray(QByteArray& outputResult, const QString& oscAddress, const QVariantList& arguments) {
    char buffer[1024];
    osc::OutboundPacketStream packet(buffer, 1024);
    // FIXME: Sending datagrams larger than 512 bytes is in general disadvised, as even if they are sent successfully,
    // they are likely to be fragmented by the IP layer before arriving at their final destination.
    // packet << osc::BeginBundleImmediate << osc::BeginMessage(oscAddress.toStdString().c_str());
    packet << osc::BeginMessage(oscAddress.toStdString().c_str());

    for (int i = 0; i < arguments.count(); ++ i) {
        QVariant argument = arguments[i];
        QMetaType::Type type = static_cast<QMetaType::Type>(argument.type());

        if (type == QMetaType::Int) {
            packet << argument.toInt();
        } else if (type == QMetaType::Float) {
            packet << argument.toFloat();
        } else if (type == QMetaType::Double) {
            packet << argument.toDouble();
        } else if (type == QMetaType::QString) {
            packet << argument.toString().toStdString().c_str();
        //} else if (type == QMetaType::QByteArray) {
        //    osc::Blob b(argument.toByteArray().constData());
        //    packet << b;
        } else if (type == QMetaType::Bool) {
            packet << argument.toBool();
        } else {
            if(flagDebug) {
                qDebug() << "unhandled OSC argument type " << argument.typeName();
            }
        }
        // TODO: implement other OSC types
    }

    // packet << osc::EndMessage << osc::EndBundle;
    packet << osc::EndMessage;
    outputResult.append(packet.Data(), packet.Size());
}
