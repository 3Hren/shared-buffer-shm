import sys
import numpy
from sharbuf.exceptions import AttachError

sys.path.append('/usr/lib')
from libpysharbuf import *

__author__ = 'EvgenySafronov <division494@gmail.com>'


class SharedBufferClient(object):
    def __init__(self, key, bufferCount, bufferSize):
        self.key = key
        self.bufferCount = bufferCount
        self.bufferSize = bufferSize
        self.reader = BufferReader(self.bufferCount, self.bufferSize)

    def isConnected(self):
        return self.reader.isAttached()

    def connect(self):
        if not self.isConnected():
            try:
                self.reader.attach(self.key)
            except RuntimeError:
                raise AttachError(self.key)

    def disconnect(self):
        pass

    def getBuffer(self, bufferId, size=None):
        if size is None:
            size = self.bufferSize
        if size <= 0:
            raise ValueError('Buffer size must be positive integer')
        array = numpy.empty(size, dtype='f')
        self.reader.getBuffer(bufferId, size, array)
        return array
