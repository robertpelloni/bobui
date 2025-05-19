// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

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

#ifndef QTESTCRASHHANDLER_H
#define QTESTCRASHHANDLER_H

#include <QtCore/qnamespace.h>
#include <QtTest/qttestglobal.h>

#include <QtCore/private/qtools_p.h>

#ifdef Q_OS_UNIX
#include <signal.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <string.h>
#include <unistd.h>
#endif

QT_BEGIN_NAMESPACE
namespace QTest {
namespace CrashHandler {
    bool alreadyDebugging();
    void blockUnixSignals();

#if !defined(Q_OS_WASM) || QT_CONFIG(thread)
    void printTestRunTime();
    void generateStackTrace();
#endif

    void maybeDisableCoreDump();
    Q_TESTLIB_EXPORT void prepareStackTrace();

#if defined(Q_OS_WIN)
    class Q_TESTLIB_EXPORT WindowsFaultHandler
    {
    public:
        WindowsFaultHandler();
    };
    using FatalSignalHandler = WindowsFaultHandler;
#elif defined(Q_OS_UNIX) && !defined(Q_OS_WASM)
    class Q_TESTLIB_EXPORT FatalSignalHandler
    {
    public:
    #  define OUR_SIGNALS(F)    \
                F(HUP)              \
                F(INT)              \
                F(QUIT)             \
                F(ABRT)             \
                F(ILL)              \
                F(BUS)              \
                F(FPE)              \
                F(SEGV)             \
                F(PIPE)             \
                F(TERM)             \
        /**/
    #  define CASE_LABEL(S)             case SIG ## S:  return QT_STRINGIFY(S);
    #  define ENUMERATE_SIGNALS(S)      SIG ## S,
                static const char *signalName(int signum) noexcept
        {
            switch (signum) {
                OUR_SIGNALS(CASE_LABEL)
            }

    #  if defined(__GLIBC_MINOR__) && (__GLIBC_MINOR__ >= 32 || __GLIBC__ > 2)
            // get the other signal names from glibc 2.32
            // (accessing the sys_sigabbrev variable causes linker warnings)
            if (const char *p = sigabbrev_np(signum))
                return p;
    #  endif
            return "???";
        }
        static constexpr std::array fatalSignals = {
            OUR_SIGNALS(ENUMERATE_SIGNALS)
        };
    #  undef CASE_LABEL
    #  undef ENUMERATE_SIGNALS

        static constexpr std::array crashingSignals = {
            // Crash signals are special, because if we return from the handler
            // without adjusting the machine state, the same instruction that
            // originally caused the crash will get re-executed and will thus cause
            // the same crash again. This is useful if our parent process logs the
            // exit result or if core dumps are enabled: the core file will point
            // to the actual instruction that crashed.
            SIGILL, SIGBUS, SIGFPE, SIGSEGV
        };
        using OldActionsArray = std::array<struct sigaction, fatalSignals.size()>;

        FatalSignalHandler();
        ~FatalSignalHandler();

    private:
        Q_DISABLE_COPY_MOVE(FatalSignalHandler)

        static OldActionsArray &oldActions();
        auto alternateStackSize();
        int setupAlternateStack();
        void freeAlternateStack();

        static void actionHandler(int signum, siginfo_t *info, void * /* ucontext */);

        [[maybe_unused]] static void regularHandler(int signum)
        {
            actionHandler(signum, nullptr, nullptr);
        }

        void *alternateStackBase = MAP_FAILED;
        static bool pauseOnCrash;
    };
#else // Q_OS_WASM or weird systems
class Q_TESTLIB_EXPORT FatalSignalHandler {};
inline void blockUnixSignals() {}
#endif // Q_OS_* choice
} // namespace CrashHandler
} // namespace QTest
QT_END_NAMESPACE

#endif // QTESTCRASHHANDLER_H
