#include "server.h"
#include <QDebug>

Server::Server(QObject *parent)
    :QObject(parent)
{

}

bool Server::setSettings(QString host, int port)
{
    _host = host;
    _port = port;
    return true;
}

void Server::setCams(ICameras * cameras)
{
    _cameras = cameras;
}

/**
 * @brief Запускаем сервер в работу
 * @return
 */
bool Server::startServer()
{
    qInfo()<<"Start server"<<this;

    _server = new QTcpServer(this);

    connect(_server, &QTcpServer::newConnection, this, &Server::newClient  );

    if (! _server->listen(_host, _port) ) {
        qWarning()<<_server->errorString();
        return false;
    }

    emit Events->doAction("ServerStarted", QVariantList()<<Events->ARG(dynamic_cast<IServer*>(this)));

    qInfo()<<"Server started";

    return true;
}

/**
 * @brief Обрабатываем новое подключение
 */
void Server::newClient()
{
    qInfo()<<"New client";

    QTcpSocket* clientSocket = _server->nextPendingConnection();

    Server_Client * client = new Server_Client(this, clientSocket, this);

     emit Events->doAction("newClient", QVariantList()<<Events->ARG(client));

    connect(clientSocket, &QTcpSocket::readyRead, client, &Server_Client::request);

    _clients.insert(client->clientID(), client);

}

ICameras * Server::getCams()
{
    return _cameras;
}

Server::~Server()
{
    qInfo()<<"Server deleted";
}
