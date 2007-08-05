/*  This file is part of the KDE project
    Copyright (C) 2007 Matthias Kretz <kretz@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

*/

#include "kiomediastream.h"
#include "kiomediastream_p.h"
#include <kdebug.h>
#include <kprotocolmanager.h>
#include <kio/filejob.h>
#include <kio/job.h>
#include <klocale.h>

namespace Phonon
{

KioMediaStream::KioMediaStream(const QUrl &url, QObject *parent)
    : AbstractMediaStream(*new KioMediaStreamPrivate(url), parent)
{
    reset();
}

void KioMediaStream::reset()
{
    Q_D(KioMediaStream);
    if (d->kiojob) {
        d->kiojob->disconnect(this);
        d->kiojob->kill();

        d->endOfDataSent = false;
        d->seeking = false;
        d->reading = false;
        d->open = false;
        d->seekPosition = 0;
    }

    if (KProtocolManager::supportsOpening(d->url)) {
        d->kiojob = KIO::open(d->url, QIODevice::ReadOnly);
        Q_ASSERT(d->kiojob);
        d->open = false;
        setStreamSeekable(true);
        connect(d->kiojob, SIGNAL(open(KIO::Job *)), this, SLOT(_k_bytestreamFileJobOpen(KIO::Job *)));
        connect(d->kiojob, SIGNAL(position(KIO::Job *, KIO::filesize_t)),
                this, SLOT(_k_bytestreamSeekDone(KIO::Job *, KIO::filesize_t)));
    } else {
        d->kiojob = KIO::get(d->url, false, false);
        Q_ASSERT(d->kiojob);
        setStreamSeekable(false);
        connect(d->kiojob, SIGNAL(totalSize(KJob *, qulonglong)),
                this, SLOT(_k_bytestreamTotalSize(KJob *,qulonglong)));
        d->kiojob->suspend();
    }

    d->kiojob->addMetaData("UserAgent", QLatin1String("KDE Phonon"));
    connect(d->kiojob, SIGNAL(data(KIO::Job *,const QByteArray &)),
            this, SLOT(_k_bytestreamData(KIO::Job *,const QByteArray &)));
    connect(d->kiojob, SIGNAL(result(KJob *)), this, SLOT(_k_bytestreamResult(KJob *)));
}

KioMediaStream::~KioMediaStream()
{
}

void KioMediaStream::needData()
{
    Q_D(KioMediaStream);
    KIO::FileJob *filejob = qobject_cast<KIO::FileJob *>(d->kiojob);
    if (filejob) {
        // while d->seeking the backend won't get any data
        if (d->seeking || !d->open) {
            d->reading = true;
        } else if (!d->reading) {
            d->reading = true;
            filejob->read(32768);
        }
    } else {
        // KIO::TransferJob
        d->kiojob->resume();
    }
}

void KioMediaStream::enoughData()
{
    Q_D(KioMediaStream);
    kDebug(600) << k_funcinfo;
    // Don't suspend when using a FileJob. The FileJob is controlled by calls to
    // FileJob::read()
    if (d->kiojob && !qobject_cast<KIO::FileJob *>(d->kiojob) && !d->kiojob->isSuspended()) {
        d->kiojob->suspend();
    } else {
        d->reading = false;
    }
}

void KioMediaStream::seekStream(qint64 position)
{
    Q_D(KioMediaStream);
    kDebug(600) << k_funcinfo << position << " = " << qulonglong(position);
    d->seeking = true;
    if (d->open) {
        KIO::FileJob *filejob = qobject_cast<KIO::FileJob *>(d->kiojob);
        filejob->seek(position);
    } else {
        d->seekPosition = position;
    }
}

void KioMediaStreamPrivate::_k_bytestreamData(KIO::Job *, const QByteArray &data)
{
    Q_Q(KioMediaStream);
    if (seeking) {
        // seek doesn't block, so don't send data to the backend until it signals us
        // that the seek is done
        kDebug(600) << k_funcinfo << "seeking: do nothing";
        return;
    }

    if (data.isEmpty()) {
        reading = false;
        if (!endOfDataSent) {
            kDebug(600) << k_funcinfo << "empty data: stopping the stream";
            endOfDataSent = true;
            q->endOfData();
        }
        return;
    }

    //kDebug(600) << k_funcinfo << "calling writeData on the Backend ByteStream " << data.size();
    q->writeData(data);
    if (reading) {
        KIO::FileJob *filejob = qobject_cast<KIO::FileJob *>(kiojob);
        Q_ASSERT(filejob);
        filejob->read(32768);
    }
}

void KioMediaStreamPrivate::_k_bytestreamResult(KJob *job)
{
    Q_Q(KioMediaStream);
    if (job->error()) {
        QString kioErrorString = job->errorString();
        kDebug(600) << "KIO Job error: " << kioErrorString;
        QObject::disconnect(kiojob, SIGNAL(data(KIO::Job *,const QByteArray &)),
                q, SLOT(bytestreamData(KIO::Job *,const QByteArray &)));
        QObject::disconnect(kiojob, SIGNAL(result(KJob *)),
                q, SLOT(bytestreamResult(KJob *)));
        KIO::FileJob *filejob = qobject_cast<KIO::FileJob *>(kiojob);
        if (filejob) {
            QObject::disconnect(kiojob, SIGNAL(open(KIO::Job *)),
                    q, SLOT(bytestreamFileJobOpen(KIO::Job *)));
            QObject::disconnect(kiojob, SIGNAL(position(KIO::Job *, KIO::filesize_t)),
                    q, SLOT(bytestreamSeekDone(KIO::Job *, KIO::filesize_t)));
        } else {
            QObject::disconnect(kiojob, SIGNAL(totalSize(KJob *, qulonglong)),
                    q, SLOT(bytestreamTotalSize(KJob *,qulonglong)));
        }
        // go to ErrorState - NormalError
        q->error(NormalError, kioErrorString);
    }
    kiojob = 0;
    endOfDataSent = true;
    q->endOfData();
    reading = false;
}

void KioMediaStreamPrivate::_k_bytestreamTotalSize(KJob *, qulonglong size)
{
    Q_Q(KioMediaStream);
    kDebug(600) << k_funcinfo << size;
    q->setStreamSize(size);
}

void KioMediaStreamPrivate::_k_cleanupByteStream()
{
    if (kiojob) {
        kiojob->kill();
        kiojob = 0;
        reading = false;
    }
}

void KioMediaStreamPrivate::_k_bytestreamFileJobOpen(KIO::Job *)
{
    Q_Q(KioMediaStream);
    open = true;
    endOfDataSent = false;
    KIO::FileJob *filejob = static_cast<KIO::FileJob *>(kiojob);
    kDebug(600) << k_funcinfo << filejob->size();
    q->setStreamSize(filejob->size());

    if (seeking) {
        filejob->seek(seekPosition);
    } else if (reading) {
        filejob->read(32768);
    }
}

void KioMediaStreamPrivate::_k_bytestreamSeekDone(KIO::Job *, KIO::filesize_t offset)
{
    kDebug(600) << k_funcinfo << offset;
    seeking = false;
    endOfDataSent = false;
    if (reading) {
        KIO::FileJob *filejob = qobject_cast<KIO::FileJob *>(kiojob);
        Q_ASSERT(filejob);
        filejob->read(32768);
    }
}

} // namespace Phonon

#include "kiomediastream.moc"
// vim: sw=4 sts=4 et tw=100
