/*  This file is part of the KDE project
    Copyright (C) 2005-2007 Matthias Kretz <kretz@kde.org>

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
#include "videodataoutput.h"
#include "videodataoutput_p.h"
#include "factory.h"
#include <QSize>

#define PHONON_CLASSNAME VideoDataOutput

namespace Phonon
{
namespace Experimental
{

VideoDataOutput::VideoDataOutput(QObject *parent)
    : QObject(parent)
    , AbstractVideoOutput(*new VideoDataOutputPrivate)
{
    K_D(VideoDataOutput);
    d->createIface();
}

void VideoDataOutputPrivate::createIface()
{
    if (backendObject)
        return;
    K_Q(VideoDataOutput);
    backendObject = Factory::createVideoDataOutput(q);
    if (backendObject)
        q->setupIface();
}

PHONON_GETTER(quint32, format, d->format)

bool VideoDataOutput::formatSupported(quint32 fourcc)
{
    QObject *backend = Factory::backend();
    if (backend)
    {
        bool ret;
        QMetaObject::invokeMethod(backend, "supportsFourcc", Q_RETURN_ARG(bool, ret), Q_ARG(quint32, fourcc));
        return ret;
    }
    return false;
}

PHONON_GETTER(int, frameRate, d->frameRate)
PHONON_SETTER(setFrameRate, frameRate, int)
PHONON_SETTER(setFormat, format, quint32)
PHONON_GETTER(QSize, frameSize, d->frameSize)

void VideoDataOutput::setFrameSize(const QSize &size, Qt::AspectRatioMode aspectRatioMode)
{
    K_D(VideoDataOutput);
    d->frameSize = size;
    d->frameAspectRatioMode = aspectRatioMode;

    if (iface())
    {
        QSize newsize;
        BACKEND_GET(QSize, newsize, "naturalFrameSize");
        newsize.scale(size, aspectRatioMode);
        BACKEND_CALL1("setFrameSize", QSize, newsize);
    }
}

void VideoDataOutput::setFrameSize(int width, int height, Qt::AspectRatioMode aspectRatioMode)
{
    setFrameSize(QSize(width, height), aspectRatioMode);
}

bool VideoDataOutputPrivate::aboutToDeleteIface()
{
    Q_ASSERT(backendObject);
    pBACKEND_GET(quint32, format, "format");

    return AbstractVideoOutputPrivate::aboutToDeleteIface();
}

void VideoDataOutput::setupIface()
{
    K_D(VideoDataOutput);
    Q_ASSERT(d->backendObject);
    AbstractVideoOutput::setupIface();

    // set up attributes
    BACKEND_CALL1("setFormat", quint32, d->format);
    //d->backendObject->setDisplayLatency(d->displayLatency);
    connect(d->backendObject, SIGNAL(frameReady(const Phonon::Experimental::VideoFrame &)),
            SIGNAL(frameReady(const Phonon::Experimental::VideoFrame &)));
    connect(d->backendObject, SIGNAL(endOfMedia()), SIGNAL(endOfMedia()));
}

} // namespace Experimental
} // namespace Phonon

#include "videodataoutput.moc"

#undef PHONON_CLASSNAME
// vim: sw=4 ts=4 tw=80
