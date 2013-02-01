#include <signal.h>

#include <iostream>

#include "SharedBufferOptionParser.h"
#include "Server.h"

#include <log4cxx/propertyconfigurator.h>

#include <QCoreApplication>
#include <QSettings>

void setSignalHandler() {
    struct sigaction sa;
    sa.sa_handler = [](int signal_id){
        std::cerr << std::endl;
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
    app.setApplicationName("SharedBufferServer");
    app.setApplicationVersion("0.1.1");
    app.setOrganizationName("Diaprom");
    app.setOrganizationDomain("www.diaprom.ru");

    QSettings settings;
    const QString &logConfigFileName = settings.value("logConfigFileName", "log.properties").toString();

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
    const BufferSize bufferSize = parser.getBufferSize();    

    Server server(key, bufferCount, bufferSize);
    server.execute();
    return app.exec();
}
