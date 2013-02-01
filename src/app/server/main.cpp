#include <signal.h>

#include <iostream>

#include <QCoreApplication>

#include "SharedBufferOptionParser.h"
#include "Server.h"

void setSignalHandler() {
    struct sigaction sa;
    sa.sa_handler = [](int signal_id){
        std::cerr << std::endl << "Received signal: " << signal_id << ". Exiting ..." << std::endl;
        qApp->quit();
    };
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, 0);
    sigaction(SIGTERM, &sa, 0);
}

int main(int argc, char **argv) {
    SharedBufferOptionParser parser;
    const int status = parser.parseArguments(argc, argv);
    if (status != SharedBufferOptionParser::SUCCESS)
        return status;

    setSignalHandler();

    const QString &key = parser.getKey();
    const BufferId bufferCount = parser.getBufferCount();
    const BufferSize bufferSize = parser.getBufferSize();

    QCoreApplication app(argc, argv);
    Server server(key, bufferCount, bufferSize);
    server.execute();
    return app.exec();
}
