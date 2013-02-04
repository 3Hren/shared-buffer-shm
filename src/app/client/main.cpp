#include <signal.h>

#include <QCoreApplication>
#include <QDebug>

#include "Pusher.h"
#include "Dumper.h"

#include <log4cxx/propertyconfigurator.h>

#include <QCoreApplication>
#include <QSettings>

void terminateHandler(int i) {
    qDebug() << i;
    qApp->quit();
}

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    app.setApplicationName("SharedBufferClient");
    app.setApplicationVersion("0.1.1");
    app.setOrganizationName("Diaprom");
    app.setOrganizationDomain("www.diaprom.ru");

    QSettings settings;
    const QString &logConfigFileName = settings.value("logConfigFileName", qApp->applicationDirPath() + "/log.properties").toString();

    log4cxx::PropertyConfigurator::configure(logConfigFileName.toStdString());
    LOG4CXX_INFO(log4cxx::Logger::getRootLogger(),
                 QString("Starting application '%1' from %2 - %3. All rights reserved.")
                 .arg(qApp->applicationName() + " " + qApp->applicationVersion())
                 .arg(qApp->organizationName())
                 .arg(qApp->organizationDomain()).toLatin1().constData());

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
        pusher.connectAndRun();
        return app.exec();
    } else if (type == 2) {
        Dumper dumper(name, buffersCount, bufferSize, timeout);
        dumper.connectAndRun();
        return app.exec();
    }

    return 2;
}
