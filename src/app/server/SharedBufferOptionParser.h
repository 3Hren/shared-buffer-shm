#pragma once

#include "SharedBufferGlobal.h"

class SharedBufferOptionParser
{    
    QString key;
    BufferId bufferCount = 0;
    BufferSize bufferSize = 0;
public:
    int parseArguments(int argc, char **argv);

    QString getKey() const;
    BufferId getBufferCount() const;
    BufferSize getBufferSize() const;

public:
    enum Status {
        SUCCESS = 0,
        ERROR_IN_COMMAND_LINE = 1,
        ERROR_UNHANDLED_EXCEPTION = 2
    };
};
