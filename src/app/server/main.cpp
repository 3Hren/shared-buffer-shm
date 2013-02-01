#include <signal.h>

#include <QCoreApplication>
#include <QDebug>

#include "Server.h"

void terminateHandler(int i) {
    qDebug() << i;
    qApp->quit();
}

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    qDebug() << "USAGE: [KEY] [BUFFERS_COUNT] [BUFFER_SIZE]";

    struct sigaction sa;
    sa.sa_handler = terminateHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, 0);
    sigaction(SIGTERM, &sa, 0);

    QString name = QVariant(argv[1]).toString();
    int buffersCount = QVariant(argv[2]).toInt();
    int bufferSize = QVariant(argv[3]).toInt();
    Server server(name, buffersCount, bufferSize);
    server.execute();
    return app.exec();
}
