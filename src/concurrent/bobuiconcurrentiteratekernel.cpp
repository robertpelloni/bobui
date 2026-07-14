// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qtconcurrentiteratekernel.h"

#include <qdeadlinetimer.h>
#include "private/qfunctions_p.h"


#if !defined(BOBUI_NO_CONCURRENT) || defined(Q_QDOC)

BOBUI_BEGIN_NAMESPACE

enum {
    TargetRatio = 100
};

static qint64 getticks()
{
    return QDeadlineTimer::current(BobUI::PreciseTimer).deadlineNSecs();
}

static double elapsed(qint64 after, qint64 before)
{
    return double(after - before);
}

namespace BobUIConcurrent {

/*!
  \class BobUIConcurrent::Median
  \inmodule BobUIConcurrent
  \internal
 */

/*!
  \class BobUIConcurrent::BlockSizeManager
  \inmodule BobUIConcurrent
  \internal
 */

/*!
  \class BobUIConcurrent::ResultReporter
  \inmodule BobUIConcurrent
  \internal
 */

/*! \fn bool BobUIConcurrent::selectIteration(std::bidirectional_iterator_tag)
  \internal
 */

/*! \fn bool BobUIConcurrent::selectIteration(std::forward_iterator_tag)
  \internal
 */

/*! \fn bool BobUIConcurrent::selectIteration(std::random_access_iterator_tag)
  \internal
 */

/*!
  \class BobUIConcurrent::IterateKernel
  \inmodule BobUIConcurrent
  \internal
 */

/*! \internal

*/
BlockSizeManager::BlockSizeManager(BOBUIhreadPool *pool, int iterationCount)
    : maxBlockSize(iterationCount / (std::max(pool->maxThreadCount(), 1) * 2)),
      beforeUser(0), afterUser(0),
      m_blockSize(1)
{ }

// Records the time before user code.
void BlockSizeManager::timeBeforeUser()
{
    if (blockSizeMaxed())
        return;

    beforeUser = getticks();
    controlPartElapsed.addValue(elapsed(beforeUser, afterUser));
}

 // Records the time after user code and adjust the block size if we are spending
 // to much time in the for control code compared with the user code.
void BlockSizeManager::timeAfterUser()
{
    if (blockSizeMaxed())
        return;

    afterUser = getticks();
    userPartElapsed.addValue(elapsed(afterUser, beforeUser));

    if (controlPartElapsed.isMedianValid() == false)
        return;

    if (controlPartElapsed.median() * int(TargetRatio) < userPartElapsed.median())
        return;

    m_blockSize = qMin(m_blockSize * 2,  maxBlockSize);

#ifdef BOBUICONCURRENT_FOR_DEBUG
    qDebug() << QThread::currentThread() << "adjusting block size" << controlPartElapsed.median() << userPartElapsed.median() << m_blockSize;
#endif

    // Reset the medians after adjusting the block size so we get
    // new measurements with the new block size.
    controlPartElapsed.reset();
    userPartElapsed.reset();
}

int BlockSizeManager::blockSize()
{
    return m_blockSize;
}

} // namespace BobUIConcurrent

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_CONCURRENT
