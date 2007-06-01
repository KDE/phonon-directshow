/*  This file is part of the KDE project
    Copyright (C) 2005 Matthias Kretz <kretz@kde.org>

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
#include "audiodataoutput.h"
#include "audiodataoutput_p.h"
#include "../factory.h"

#define PHONON_CLASSNAME AudioDataOutput

namespace Phonon
{
namespace Experimental
{

PHONON_HEIR_IMPL(AbstractAudioOutput)

PHONON_GETTER(Phonon::Experimental::AudioDataOutput::Format, format, d->format)
PHONON_GETTER(int, dataSize, d->dataSize)
PHONON_GETTER(int, sampleRate, -1)
PHONON_SETTER(setFormat, format, Phonon::Experimental::AudioDataOutput::Format)
PHONON_SETTER(setDataSize, dataSize, int)

bool AudioDataOutputPrivate::aboutToDeleteBackendObject()
{
    Q_ASSERT(m_backendObject);
    pBACKEND_GET(Phonon::Experimental::AudioDataOutput::Format, format, "format");
    pBACKEND_GET(int, dataSize, "dataSize");

    return AbstractAudioOutputPrivate::aboutToDeleteBackendObject();
}

void AudioDataOutputPrivate::setupBackendObject()
{
    Q_Q(AudioDataOutput);
    Q_ASSERT(m_backendObject);
    AbstractAudioOutputPrivate::setupBackendObject();

    // set up attributes
    pBACKEND_CALL1("setFormat", Phonon::Experimental::AudioDataOutput::Format, format);
    pBACKEND_CALL1("setDataSize", int, dataSize);
    QObject::connect(m_backendObject,
            SIGNAL(dataReady(const QMap<Phonon::Experimental::AudioDataOutput::Channel, QVector<qint16> > &)),
            q, SIGNAL(dataReady(const QMap<Phonon::Experimental::AudioDataOutput::Channel, QVector<qint16> > &)));
    QObject::connect(m_backendObject,
            SIGNAL(dataReady(const QMap<Phonon::Experimental::AudioDataOutput::Channel, QVector<float> > &)),
            q, SIGNAL(dataReady(const QMap<Phonon::Experimental::AudioDataOutput::Channel, QVector<float> > &)));
    QObject::connect(m_backendObject, SIGNAL(endOfMedia(int)), q, SIGNAL(endOfMedia(int)));
}

} // namespace Experimental
} // namespace Phonon

#include "audiodataoutput.moc"

#undef PHONON_CLASSNAME
// vim: sw=4 ts=4 tw=80
