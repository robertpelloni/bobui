// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// Qt-Security score:significant reason:default

#ifndef QRANDOMACCESSASYNCFILE_P_P_H
#define QRANDOMACCESSASYNCFILE_P_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qrandomaccessasyncfile_p.h"

#include <QtCore/private/qobject_p.h>

#include <QtCore/qstring.h>

#ifdef QT_RANDOMACCESSASYNCFILE_THREAD

#include <QtCore/private/qfsfileengine_p.h>

#include <QtCore/qfuturewatcher.h>
#include <QtCore/qmutex.h>
#include <QtCore/qqueue.h>

#endif // QT_RANDOMACCESSASYNCFILE_THREAD

QT_BEGIN_NAMESPACE

class QRandomAccessAsyncFilePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QRandomAccessAsyncFile)
    Q_DISABLE_COPY_MOVE(QRandomAccessAsyncFilePrivate)
public:
    QRandomAccessAsyncFilePrivate(decltype(QObjectPrivateVersion) version = QObjectPrivateVersion);
    ~QRandomAccessAsyncFilePrivate() override;

    static QRandomAccessAsyncFilePrivate *get(QRandomAccessAsyncFile *file)
    { return file->d_func(); }

    void init();
    void cancelAndWait(QIOOperation *op);

    bool open(const QString &path, QIODeviceBase::OpenMode mode);
    void close();
    qint64 size() const;

    [[nodiscard]] QIOReadOperation *read(qint64 offset, qint64 maxSize);
    [[nodiscard]] QIOWriteOperation *write(qint64 offset, const QByteArray &data);
    [[nodiscard]] QIOWriteOperation *write(qint64 offset, QByteArray &&data);

    [[nodiscard]] QIOVectoredReadOperation *
    readInto(qint64 offset, QSpan<std::byte> buffer);
    [[nodiscard]] QIOVectoredWriteOperation *
    writeFrom(qint64 offset, QSpan<const std::byte> buffer);

    [[nodiscard]] QIOVectoredReadOperation *
    readInto(qint64 offset, QSpan<const QSpan<std::byte>> buffers);
    [[nodiscard]] QIOVectoredWriteOperation *
    writeFrom(qint64 offset, QSpan<const QSpan<const std::byte>> buffers);

private:
#ifdef QT_RANDOMACCESSASYNCFILE_THREAD
public:
    struct OperationResult
    {
        qint64 bytesProcessed; // either read or written
        QIOOperation::Error error;
    };

private:
    mutable QBasicMutex m_engineMutex;
    std::unique_ptr<QFSFileEngine> m_engine;
    QFutureWatcher<OperationResult> m_watcher;

    QQueue<QPointer<QIOOperation>> m_operations;
    QPointer<QIOOperation> m_currentOperation;
    qsizetype numProcessedBuffers = 0;

    void executeNextOperation();
    void processBufferAt(qsizetype idx);
    void operationComplete();
#endif
};

QT_END_NAMESPACE

#endif // QRANDOMACCESSASYNCFILE_P_P_H
