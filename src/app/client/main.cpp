#include <signal.h>

#include <QCoreApplication>
#include <QDebug>

#include "Pusher.h"
#include "Dumper.h"

void terminateHandler(int i) {
    qDebug() << i;
    qApp->quit();
}

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    qDebug() << "USAGE: [TYPE](1 - Pusher, 2 - Dumper) [NAME] [BUFFERS_COUNT] [BUFFER_SIZE] [TIMEOUT]";

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

    if (type == 1){
        Pusher pusher(name, buffersCount, bufferSize, timeout);
        pusher.execute();
        return app.exec();
    } else if (type == 2) {
        Dumper dumper(name, buffersCount, bufferSize, timeout);
        dumper.execute();
        return app.exec();
    }

    return 2;
}
