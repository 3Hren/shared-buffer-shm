#include <QCoreApplication>

#include <QDebug>

#include <signal.h>

#include "Server.h"
#include "Pusher.h"
#include "Dumper.h"

void terminateHandler(int i) {
    qDebug() << i;
    qApp->quit();
}

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    qDebug() << "USAGE: [TYPE](1 - Server, 2 - Pusher, 3 - Dumper) [NAME] [BUFFERS_COUNT] [BUFFER_SIZE] [TIMEOUT](not neccessary for server)";

    struct sigaction sa;
    sa.sa_handler = terminateHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, 0);
    sigaction(SIGTERM, &sa, 0);

    int type = QVariant(argv[1]).toUInt();
    QString name = QVariant(argv[2]).toString();
    int buffersCount = QVariant(argv[3]).toInt();
    int bufferSize = QVariant(argv[4]).toInt();
    int timeout = QVariant(argv[5]).toInt();
    if (type == 1) {
        Server server(name, buffersCount, bufferSize);
        server.execute();
        return app.exec();
    } else if (type == 2){
        Pusher pusher(name, buffersCount, bufferSize, timeout);
        pusher.execute();
        return app.exec();
    } else if (type == 3) {
        Dumper dumper(name, buffersCount, bufferSize, timeout);
        dumper.execute();
        return app.exec();
    }

    return 2;
}

/*! @todo: optimizations
 * Расставить Q_UNLIKELY
 */
