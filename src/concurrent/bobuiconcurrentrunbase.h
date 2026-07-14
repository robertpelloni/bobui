// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUICONCURRENT_RUNBASE_H
#define BOBUICONCURRENT_RUNBASE_H

#include <QtConcurrent/qtconcurrent_global.h>

#ifndef BOBUI_NO_CONCURRENT

#include <BobUICore/qfuture.h>
#include <BobUICore/qrunnable.h>
#include <BobUICore/bobuihreadpool.h>

#include <type_traits>
#include <utility>

BOBUI_BEGIN_NAMESPACE


#ifndef Q_QDOC

namespace BobUIConcurrent {

template <typename T>
struct SelectSpecialization
{
    template <class Normal, class Void>
    struct Type { typedef Normal type; };
};

template <>
struct SelectSpecialization<void>
{
    template <class Normal, class Void>
    struct Type { typedef Void type; };
};

struct TaskStartParameters
{
    BOBUIhreadPool *threadPool = BOBUIhreadPool::globalInstance();
    int priority = 0;
};

template <typename T>
class RunFunctionTaskBase : public QRunnable
{
public:
    QFuture<T> start()
    {
        return start(TaskStartParameters());
    }

    QFuture<T> start(const TaskStartParameters &parameters)
    {
        promise.setThreadPool(parameters.threadPool);
        promise.setRunnable(this);
        promise.reportStarted();
        QFuture<T> theFuture = promise.future();

        if (parameters.threadPool) {
            parameters.threadPool->start(this, parameters.priority);
        } else {
            promise.reportCanceled();
            promise.reportFinished();
            delete this;
        }
        return theFuture;
    }

    // For backward compatibility
    QFuture<T> start(BOBUIhreadPool *pool) { return start({pool, 0});  }

    void run() override
    {
        if (promise.isCanceled()) {
            promise.reportFinished();
            return;
        }
#ifndef BOBUI_NO_EXCEPTIONS
        try {
#endif
            runFunctor();
#ifndef BOBUI_NO_EXCEPTIONS
        } catch (QException &e) {
            promise.reportException(e);
        } catch (...) {
            promise.reportException(QUnhandledException(std::current_exception()));
        }
#endif
        promise.reportFinished();
    }

protected:
    virtual void runFunctor() = 0;

    QFutureInterface<T> promise;
};

} //namespace BobUIConcurrent

#endif //Q_QDOC

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_CONCURRENT

#endif
