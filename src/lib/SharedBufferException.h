#pragma once

#include <exception>
#include <QString>

class SharedBufferNotAttachedException : public std::exception {
    const QString reason;
public:
    SharedBufferNotAttachedException(const QString &reason) throw() : std::exception(), reason(reason) {}
    ~SharedBufferNotAttachedException() throw() {}

    const char *what() const throw() {
        return reason.toUtf8().constData();
    }
};

class SharedBufferAlreadyAttachedException : public std::exception {
    const QString reason;
public:
    SharedBufferAlreadyAttachedException() throw() : std::exception(), reason("Shared buffer already attached") {}
    ~SharedBufferAlreadyAttachedException() throw() {}

    const char *what() const throw() {
        return reason.toUtf8().constData();
    }
};
