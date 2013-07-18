#pragma once

#include <stdexcept>

#include <QString>

class AttachException : public std::runtime_error {
public:
    AttachException(const QString &reason) throw() : std::runtime_error(reason.toStdString()) {}
};

class SharedBufferNotAttachedException : public AttachException {
public:
    SharedBufferNotAttachedException(const QString &reason) throw() : AttachException(reason) {}
};

class SharedBufferAlreadyAttachedException : public AttachException {
public:
    SharedBufferAlreadyAttachedException() throw() : AttachException("Shared buffer already attached") {}
};

typedef std::invalid_argument IllegalValueException;
typedef std::invalid_argument IllegalStateException;
