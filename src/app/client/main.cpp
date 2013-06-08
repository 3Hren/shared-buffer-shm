#include <signal.h>

#include <QCoreApplication>
#include <QDebug>

#include "_Pusher.h"
#include "_Dumper.h"
#include "BeanFactory.h"

#include <boost/scoped_ptr.hpp>
#include <log4cxx/propertyconfigurator.h>

#include <QCoreApplication>
#include <QSettings>

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

    qDebug() << "USAGE: [TYPE](1 - _Pusher, 2 - _Dumper) [NAME] [BUFFERS_COUNT] [BUFFER_SIZE] [TIMEOUT]";

    struct sigaction sa;
    sa.sa_handler = [](int signal_id){
        LOG4CXX_INFO(log4cxx::Logger::getRootLogger(), "Received signal: " << signal_id << " - " << strsignal(signal_id) << ". Exiting ...");
        qApp->quit();
    };
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, 0);
    sigaction(SIGTERM, &sa, 0);

    int type = QVariant(argv[1]).toUInt();
    QString name = QVariant(argv[2]).toString();
    int buffersCount = QVariant(argv[3]).toInt();
    int bufferSize = QVariant(argv[4]).toInt();
    int timeout = QVariant(argv[5]).toInt();

    boost::scoped_ptr<BeanFactory>factory(new BeanFactory());
    BeanFactory::registerFactory(factory.get());
    if (type == 1){
        _Pusher pusher(name, buffersCount, bufferSize, timeout);
        pusher.execute();
        return app.exec();
    } else if (type == 2) {
        _Dumper dumper(name, buffersCount, bufferSize, timeout);
        dumper.connectAndRun();
        return app.exec();
    }

    return 2;
}
