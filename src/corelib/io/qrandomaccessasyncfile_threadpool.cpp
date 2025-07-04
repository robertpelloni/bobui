// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// Qt-Security score:significant reason:default

#include "qrandomaccessasyncfile_p_p.h"

#include "qiooperation_p.h"
#include "qiooperation_p_p.h"

#include <QtCore/qfuture.h>
#include <QtCore/qthread.h>
#include <QtCore/qthreadpool.h>

QT_REQUIRE_CONFIG(thread);
QT_REQUIRE_CONFIG(future);

QT_BEGIN_NAMESPACE

namespace {

// We cannot use Q_GLOBAL_STATIC(QThreadPool, foo) because the Windows
// implementation raises a qWarning in its destructor when used as a global
// static, and this warning leads to a crash on Windows CI. Cannot reproduce
// the crash locally, so cannot really fix the issue :(
// This class should act like a global thread pool, but it'll have a sort of
// ref counting, and will be created/destroyed by QRAAFP instances.
class SharedThreadPool
{
public:
    void ref()
    {
        QMutexLocker locker(&m_mutex);
        if (m_refCount == 0) {
            Q_ASSERT(!m_pool);
            m_pool = new QThreadPool;
        }
        ++m_refCount;
    }

    void deref()
    {
        QMutexLocker locker(&m_mutex);
        Q_ASSERT(m_refCount);
        if (--m_refCount == 0) {
            delete m_pool;
            m_pool = nullptr;
        }
    }

    QThreadPool *operator()()
    {
        QMutexLocker locker(&m_mutex);
        Q_ASSERT(m_refCount > 0);
        return m_pool;
    }

private:
    QBasicMutex m_mutex;
    QThreadPool *m_pool = nullptr;
    quint64 m_refCount = 0;
};

static SharedThreadPool asyncFileThreadPool;

} // anonymous namespace

QRandomAccessAsyncFilePrivate::QRandomAccessAsyncFilePrivate(decltype(QObjectPrivateVersion) version) :
    QObjectPrivate(version)
{
    asyncFileThreadPool.ref();
}

QRandomAccessAsyncFilePrivate::~QRandomAccessAsyncFilePrivate()
{
    asyncFileThreadPool.deref();
}

void QRandomAccessAsyncFilePrivate::init()
{
    QObject::connect(&m_watcher, &QFutureWatcherBase::finished, q_ptr, [this]{
        operationComplete();
    });
    QObject::connect(&m_watcher, &QFutureWatcherBase::canceled, q_ptr, [this]{
        operationComplete();
    });
}

void QRandomAccessAsyncFilePrivate::cancelAndWait(QIOOperation *op)
{
    if (op == m_currentOperation) {
        m_currentOperation = nullptr; // to discard the result
        m_watcher.cancel(); // might have no effect
        m_watcher.waitForFinished();
    } else {
        m_operations.removeAll(op);
    }
}

bool QRandomAccessAsyncFilePrivate::open(const QString &path, QIODeviceBase::OpenMode mode)
{
    QMutexLocker locker(&m_engineMutex);
    if (m_engine) {
        // already opened!
        return false;
    }

    m_engine = std::make_unique<QFSFileEngine>(path);
    mode |= QIODeviceBase::Unbuffered;
    return m_engine->open(mode, std::nullopt);
}

void QRandomAccessAsyncFilePrivate::close()
{
    // all the operations should be aborted
    for (const auto &op : std::as_const(m_operations)) {
        if (op) {
            auto *priv = QIOOperationPrivate::get(op.get());
            priv->setError(QIOOperation::Error::Aborted);
        }
    }
    m_operations.clear();

    // Wait until the current operation is complete
    if (m_currentOperation) {
        auto *priv = QIOOperationPrivate::get(m_currentOperation.get());
        priv->setError(QIOOperation::Error::Aborted);
        cancelAndWait(m_currentOperation.get());
    }

    QMutexLocker locker(&m_engineMutex);
    if (m_engine) {
        m_engine->close();
        m_engine.reset();
    }
}

qint64 QRandomAccessAsyncFilePrivate::size() const
{
    QMutexLocker locker(&m_engineMutex);
    if (m_engine)
        return m_engine->size();
    return -1;
}

QIOReadOperation *QRandomAccessAsyncFilePrivate::read(qint64 offset, qint64 maxSize)
{
    QByteArray array;
    array.resizeForOverwrite(maxSize);
    auto *dataStorage = new QtPrivate::QIOOperationDataStorage(std::move(array));

    auto *priv = new QIOOperationPrivate(dataStorage);
    priv->offset = offset;
    priv->type = QIOOperation::Type::Read;

    auto *op = new QIOReadOperation(*priv, q_ptr);
    m_operations.append(op);
    executeNextOperation();
    return op;
}

QIOWriteOperation *
QRandomAccessAsyncFilePrivate::write(qint64 offset, const QByteArray &data)
{
    auto *dataStorage = new QtPrivate::QIOOperationDataStorage(data);

    auto *priv = new QIOOperationPrivate(dataStorage);
    priv->offset = offset;
    priv->type = QIOOperation::Type::Write;

    auto *op = new QIOWriteOperation(*priv, q_ptr);
    m_operations.append(op);
    executeNextOperation();
    return op;
}

QIOWriteOperation *
QRandomAccessAsyncFilePrivate::write(qint64 offset, QByteArray &&data)
{
    auto *dataStorage = new QtPrivate::QIOOperationDataStorage(std::move(data));

    auto *priv = new QIOOperationPrivate(dataStorage);
    priv->offset = offset;
    priv->type = QIOOperation::Type::Write;

    auto *op = new QIOWriteOperation(*priv, q_ptr);
    m_operations.append(op);
    executeNextOperation();
    return op;
}

QIOVectoredReadOperation *
QRandomAccessAsyncFilePrivate::readInto(qint64 offset, QSpan<std::byte> buffer)
{
    auto *dataStorage =
            new QtPrivate::QIOOperationDataStorage(QSpan<const QSpan<std::byte>>{buffer});

    auto *priv = new QIOOperationPrivate(dataStorage);
    priv->offset = offset;
    priv->type = QIOOperation::Type::Read;

    auto *op = new QIOVectoredReadOperation(*priv, q_ptr);
    m_operations.append(op);
    executeNextOperation();
    return op;
}

QIOVectoredWriteOperation *
QRandomAccessAsyncFilePrivate::writeFrom(qint64 offset, QSpan<const std::byte> buffer)
{
    auto *dataStorage =
            new QtPrivate::QIOOperationDataStorage(QSpan<const QSpan<const std::byte>>{buffer});

    auto *priv = new QIOOperationPrivate(dataStorage);
    priv->offset = offset;
    priv->type = QIOOperation::Type::Write;

    auto *op = new QIOVectoredWriteOperation(*priv, q_ptr);
    m_operations.append(op);
    executeNextOperation();
    return op;
}

QIOVectoredReadOperation *
QRandomAccessAsyncFilePrivate::readInto(qint64 offset, QSpan<const QSpan<std::byte>> buffers)
{
    auto *dataStorage = new QtPrivate::QIOOperationDataStorage(buffers);

    auto *priv = new QIOOperationPrivate(dataStorage);
    priv->offset = offset;
    priv->type = QIOOperation::Type::Read;

    auto *op = new QIOVectoredReadOperation(*priv, q_ptr);
    m_operations.append(op);
    executeNextOperation();
    return op;
}

QIOVectoredWriteOperation *
QRandomAccessAsyncFilePrivate::writeFrom(qint64 offset, QSpan<const QSpan<const std::byte>> buffers)
{
    auto *dataStorage = new QtPrivate::QIOOperationDataStorage(buffers);

    auto *priv = new QIOOperationPrivate(dataStorage);
    priv->offset = offset;
    priv->type = QIOOperation::Type::Write;

    auto *op = new QIOVectoredWriteOperation(*priv, q_ptr);
    m_operations.append(op);
    executeNextOperation();
    return op;
}

static QRandomAccessAsyncFilePrivate::OperationResult
executeRead(QFSFileEngine *engine, QBasicMutex *mutex, qint64 offset, char *buffer, qint64 maxSize)
{
    QRandomAccessAsyncFilePrivate::OperationResult result{0, QIOOperation::Error::None};

    QMutexLocker locker(mutex);
    if (engine) {
        if (engine->seek(offset)) {
            qint64 bytesRead = engine->read(buffer, maxSize);
            if (bytesRead >= 0)
                result.bytesProcessed = bytesRead;
            else
                result.error = QIOOperation::Error::Read;
        } else {
            result.error = QIOOperation::Error::IncorrectOffset;
        }
    } else {
        result.error = QIOOperation::Error::FileNotOpen;
    }
    return result;
}

static QRandomAccessAsyncFilePrivate::OperationResult
executeWrite(QFSFileEngine *engine, QBasicMutex *mutex, qint64 offset,
             const char *buffer, qint64 size)
{
    QRandomAccessAsyncFilePrivate::OperationResult result{0, QIOOperation::Error::None};

    QMutexLocker locker(mutex);
    if (engine) {
        if (engine->seek(offset)) {
            qint64 written = engine->write(buffer, size);
            if (written >= 0)
                result.bytesProcessed = written;
            else
                result.error = QIOOperation::Error::Write;
        } else {
            result.error = QIOOperation::Error::IncorrectOffset;
        }
    } else {
        result.error = QIOOperation::Error::FileNotOpen;
    }
    return result;
}

void QRandomAccessAsyncFilePrivate::executeNextOperation()
{
    if (m_currentOperation.isNull()) {
        // start next
        if (!m_operations.isEmpty()) {
            m_currentOperation = m_operations.takeFirst();
            numProcessedBuffers = 0;
            processBufferAt(numProcessedBuffers);
        }
    }
}

void QRandomAccessAsyncFilePrivate::processBufferAt(qsizetype idx)
{
    Q_ASSERT(!m_currentOperation.isNull());
    auto *priv = QIOOperationPrivate::get(m_currentOperation.get());
    auto &dataStorage = priv->dataStorage;
    // if we do not use span buffers, we have only one buffer
    Q_ASSERT(dataStorage->containsReadSpans()
             || dataStorage->containsWriteSpans()
             || idx == 0);
    if (priv->type == QIOOperation::Type::Read) {
        qint64 maxSize = -1;
        char *buf = nullptr;
        if (dataStorage->containsReadSpans()) {
            auto &readBuffers = dataStorage->getReadSpans();
            Q_ASSERT(readBuffers.size() > idx);
            maxSize = readBuffers[idx].size_bytes();
            buf = reinterpret_cast<char *>(readBuffers[idx].data());
        } else {
            Q_ASSERT(dataStorage->containsByteArray());
            auto &array = dataStorage->getByteArray();
            maxSize = array.size();
            buf = array.data();
        }
        Q_ASSERT(maxSize >= 0);

        qint64 offset = priv->offset;
        if (idx != 0)
            offset += priv->processed;
        QBasicMutex *mutexPtr = &m_engineMutex;
        auto op = [engine = m_engine.get(), buf, maxSize, offset, mutexPtr] {
            return executeRead(engine, mutexPtr, offset, buf, maxSize);
        };

        QFuture<OperationResult> f =
                QtFuture::makeReadyVoidFuture().then(asyncFileThreadPool(), op);
        m_watcher.setFuture(f);
    } else if (priv->type == QIOOperation::Type::Write) {
        qint64 size = -1;
        const char *buf = nullptr;
        if (dataStorage->containsWriteSpans()) {
            const auto &writeBuffers = dataStorage->getWriteSpans();
            Q_ASSERT(writeBuffers.size() > idx);
            size = writeBuffers[idx].size_bytes();
            buf = reinterpret_cast<const char *>(writeBuffers[idx].data());
        } else {
            Q_ASSERT(dataStorage->containsByteArray());
            const auto &array = dataStorage->getByteArray();
            size = array.size();
            buf = array.constData();
        }
        Q_ASSERT(size >= 0);

        qint64 offset = priv->offset;
        if (idx != 0)
            offset += priv->processed;
        QBasicMutex *mutexPtr = &m_engineMutex;
        auto op = [engine = m_engine.get(), buf, size, offset, mutexPtr] {
            return executeWrite(engine, mutexPtr, offset, buf, size);
        };

        QFuture<OperationResult> f =
                QtFuture::makeReadyVoidFuture().then(asyncFileThreadPool(), op);
        m_watcher.setFuture(f);
    }
}

void QRandomAccessAsyncFilePrivate::operationComplete()
{
    // TODO: if one of the buffers was read/written with an error,
    // stop processing immediately
    if (m_currentOperation && !m_watcher.isCanceled()) {
        OperationResult res = m_watcher.future().result();
        auto *priv = QIOOperationPrivate::get(m_currentOperation.get());
        auto &dataStorage = priv->dataStorage;
        qsizetype expectedBuffersCount = 1;
        bool needProcessNext = false;
        if (priv->type == QIOOperation::Type::Read) {
            if (dataStorage->containsReadSpans()) {
                auto &readBuffers = dataStorage->getReadSpans();
                expectedBuffersCount = readBuffers.size();
                Q_ASSERT(numProcessedBuffers < expectedBuffersCount);
                const qsizetype unusedBytes =
                        readBuffers[numProcessedBuffers].size_bytes() - res.bytesProcessed;
                readBuffers[numProcessedBuffers].chop(unusedBytes);
            } else {
                Q_ASSERT(dataStorage->containsByteArray());
                Q_ASSERT(numProcessedBuffers == 0);
                auto &array = dataStorage->getByteArray();
                array.resize(res.bytesProcessed);
            }
            priv->appendBytesProcessed(res.bytesProcessed);
            needProcessNext = (++numProcessedBuffers < expectedBuffersCount);
            if (!needProcessNext)
                priv->operationComplete(res.error);
        } else if (priv->type == QIOOperation::Type::Write) {
            if (dataStorage->containsWriteSpans())
                expectedBuffersCount = dataStorage->getWriteSpans().size();
            Q_ASSERT(numProcessedBuffers < expectedBuffersCount);
            needProcessNext = (++numProcessedBuffers < expectedBuffersCount);
            priv->appendBytesProcessed(res.bytesProcessed);
            if (!needProcessNext)
                priv->operationComplete(res.error);
        }
        if (needProcessNext) {
            // keep executing this command
            processBufferAt(numProcessedBuffers);
            return;
        } else {
            m_currentOperation = nullptr;
        }
    }
    executeNextOperation();
}

QT_END_NAMESPACE
