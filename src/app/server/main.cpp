#include <signal.h>

#include <iostream>

#include <log4cxx/propertyconfigurator.h>

#include <QCoreApplication>
#include <QSettings>

#include "SharedBufferOptionParser.h"
#include "SharedBufferServer.h"
#include "config.hpp"

void setSignalHandler() {
    struct sigaction sa;
    sa.sa_handler = [](int signal_id){
        LOG4CXX_INFO(log4cxx::Logger::getRootLogger(), "Received signal: " << signal_id << " - " << strsignal(signal_id) << ". Exiting ...");
        qApp->quit();
    };
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, 0);
    sigaction(SIGTERM, &sa, 0);
}

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    app.setApplicationName("sharbuf-server");
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

    SharedBufferOptionParser parser;
    const int status = parser.parseArguments(argc, argv);
    if (status != SharedBufferOptionParser::SUCCESS)
        return status;

    setSignalHandler();

    const QString &key = parser.getKey();
    const BufferId bufferCount = parser.getBufferCount();
    const BufferPos bufferSize = parser.getBufferSize();    

    SharedBufferServer server(key, bufferCount, bufferSize);
    server.execute();
    return app.exec();
}
