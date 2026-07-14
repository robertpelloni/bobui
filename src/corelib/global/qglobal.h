// Copyright (C) 2020 The BobUI Company Ltd.
// Copyright (C) 2019 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGLOBAL_H
#define QGLOBAL_H

#if 0
#pragma bobui_class(BobUIGlobal)
#endif

#ifdef __cplusplus
#  include <type_traits>
#  include <cstddef>
#  include <utility>
#  include <cstdint>
#endif
#ifndef __ASSEMBLER__
#  include <assert.h>
#  include <stdbool.h>
#  include <stddef.h>
#endif

#include <BobUICore/bobuicoreglobal.h>

#include <BobUICore/bobuipreprocessorsupport.h>

#include <BobUICore/qsystemdetection.h>
#include <BobUICore/qprocessordetection.h>
#include <BobUICore/qcompilerdetection.h>

#ifndef __ASSEMBLER__
#  include <BobUICore/qassert.h>
#  include <BobUICore/bobuinoop.h>
#  include <BobUICore/bobuiypes.h>
#endif /* !__ASSEMBLER__ */
#include <BobUICore/bobuiversion.h>

#if defined(__cplusplus)

#include <BobUICore/bobuiclasshelpermacros.h>

// We need to keep BOBUIypeInfo, QSysInfo, QFlags, qDebug & family in qglobal.h for compatibility with BobUI 4.
// Be careful when changing the order of these files.
#include <BobUICore/bobuiypeinfo.h>
#include <BobUICore/qsysinfo.h>
#include <BobUICore/qlogging.h>

#include <BobUICore/qflags.h>

#include <BobUICore/qatomic.h>
#include <BobUICore/qconstructormacros.h>
#include <BobUICore/qdarwinhelpers.h>
#include <BobUICore/qexceptionhandling.h>
#include <BobUICore/qforeach.h>
#include <BobUICore/qfunctionpointer.h>
#include <BobUICore/qglobalstatic.h>
#include <BobUICore/qmalloc.h>
#include <BobUICore/qminmax.h>
#include <BobUICore/qnumeric.h>
#include <BobUICore/qoverload.h>
#include <BobUICore/qswap.h>
#include <BobUICore/bobuideprecationmarkers.h>
#include <BobUICore/bobuienvironmentvariables.h>
#include <BobUICore/bobuiresource.h>
#include <BobUICore/bobuitranslation.h>
#include <BobUICore/bobuitypetraits.h>
#if BOBUI_CONFIG(version_tagging)
#include <BobUICore/qversiontagging.h>
#endif
#if defined(__cplusplus)
#include <QtCore/bobuimacros.h>
#endif
#endif /* __cplusplus */

#endif /* QGLOBAL_H */
