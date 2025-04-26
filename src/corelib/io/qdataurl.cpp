// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qplatformdefs.h"
#include "qurl.h"
#include "private/qdataurl_p.h"

QT_BEGIN_NAMESPACE

using namespace Qt::Literals;

/*!
    \internal

    Decode a data: URL into its mimetype and payload. Returns a null string if
    the URL could not be decoded.
*/
Q_CORE_EXPORT bool qDecodeDataUrl(const QUrl &uri, QString &mimeType, QByteArray &payload)
{
    /* https://www.rfc-editor.org/rfc/rfc2397.html

        data:[<mediatype>][;base64],<data>
        dataurl    := "data:" [ mediatype ] [ ";base64" ] "," data
        mediatype  := [ type "/" subtype ] *( ";" parameter )
        data       := *urlchar
        parameter  := attribute "=" value
    */

    if (uri.scheme() != "data"_L1 || !uri.host().isEmpty())
        return false;

    // the following would have been the correct thing, but
    // reality often differs from the specification. People have
    // data: URIs with ? and #
    //QByteArray data = QByteArray::fromPercentEncoding(uri.path(QUrl::FullyEncoded).toLatin1());
    const QByteArray dataArray =
            QByteArray::fromPercentEncoding(uri.url(QUrl::FullyEncoded | QUrl::RemoveScheme).toLatin1());
    auto data = QLatin1StringView{dataArray};

    // parse it:
    const qsizetype pos = data.indexOf(u',');
    if (pos != -1) {
        payload = QByteArray{data.sliced(pos + 1)};
        data.truncate(pos);
        data = data.trimmed();

        // find out if the payload is encoded in Base64
        constexpr auto base64 = ";base64"_L1; // per the RFC, at the end of `data`
        if (data.endsWith(base64, Qt::CaseInsensitive)) {
            auto r = QByteArray::fromBase64Encoding(std::move(payload));
            if (!r) {
                // just in case someone uses `payload` without checking the returned bool
                payload = {};
                return false; // decoding failed
            }
            payload = std::move(r.decoded);
            data.chop(base64.size());
        }

        QLatin1StringView mime;
        QLatin1StringView charsetParam;
        constexpr auto charset = "charset"_L1;
        bool first = true;
        for (auto part : qTokenize(data, u';', Qt::SkipEmptyParts)) {
            part = part.trimmed();
            if (first) {
                if (part.contains(u'/'))
                    mime = part;
                first = false;
            }
            // Minimal changes, e.g. if it's "charset=;" or "charset;" without
            // an encoding, leave it as-is
            if (part.startsWith(charset, Qt::CaseInsensitive))
                charsetParam = part;

            if (!mime.isEmpty() && !charsetParam.isEmpty())
                break;
        }

        if (mime.isEmpty()) {
            mime = "text/plain"_L1;
            if (charsetParam.isEmpty())
                charsetParam = "charset=US-ASCII"_L1;
        }
        if (!charsetParam.isEmpty())
            mimeType = mime + u';' + charsetParam;
        else
            mimeType = mime;

        return true;
    }

    return false;
}

QT_END_NAMESPACE
