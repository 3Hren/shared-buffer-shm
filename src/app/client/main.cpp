#include "config.hpp"

#include <signal.h>

#include <boost/scoped_ptr.hpp>

#include <log4cxx/propertyconfigurator.h>

#include <QCoreApplication>
#include <QCoreApplication>
#include <QSettings>

#include "BeanFactory.h"
#include "Pusher.hpp"
#include "Dumper.hpp"

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    app.setApplicationName("sharbuf-client");
    app.setApplicationVersion(QString("%1.%2.%3").arg(SHARBUF_VERSION_MAJOR).arg(SHARBUF_VERSION_MINOR).arg(SHARBUF_VERSION_PATCH));
    app.setOrganizationName("diaprom");
    app.setOrganizationDomain("www.diaprom.ru");

    QSettings settings;
    const QString &logConfigFileName = settings.value("logConfigFileName", "/usr/share/sharbuf/log.properties").toString();

    log4cxx::PropertyConfigurator::configure(logConfigFileName.toStdString());
    LOG4CXX_INFO(log4cxx::Logger::getRootLogger(),
                 QString("Starting application '%1' from %2 - %3. All rights reserved.")
                 .arg(qApp->applicationName() + " " + qApp->applicationVersion())
                 .arg(qApp->organizationName())
                 .arg(qApp->organizationDomain()).toLatin1().constData());

    std::cerr << "USAGE: [TYPE](1 - _Pusher, 2 - _Dumper) [NAME] [BUFFERS_COUNT] [BUFFER_SIZE] [TIMEOUT]" << std::endl;

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
        Pusher pusher(name, buffersCount, bufferSize, timeout);
        pusher.execute();
        return app.exec();
    } else if (type == 2) {
        Dumper dumper(name, buffersCount, bufferSize, timeout);
        dumper.connectAndRun();
        return app.exec();
    }

    return 2;
}
