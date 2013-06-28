__author__ = 'EvgenySafronov <division494@gmail.com>'


class ConnectionError(Exception):
    pass


class AttachError(ConnectionError):
    def __init__(self, key):
        ConnectionError.__init__(self, 'Can\'t attach to shared memory segment with key={key}'.format(key=key))