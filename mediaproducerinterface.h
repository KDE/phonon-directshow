/*  This file is part of the KDE project
    Copyright (C) 2006 Matthias Kretz <kretz@kde.org>

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

#ifndef PHONON_MEDIAPRODUCERINTERFACE_H
#define PHONON_MEDIAPRODUCERINTERFACE_H

#include "phononnamespace.h"
#include <Qt/qglobal.h>

class QStringList;
class QString;
class QObject;

namespace Phonon
{

/**
 * \short Backend interface for media source classes.
 *
 * Backend classes providing MediaObject, MediaQueue, ByteStream, AvCapture,
 * etc. need to implement this interface.
 *
 * \author Matthias Kretz <kretz@kde.org>
 * \see AbstractMediaProducer
 */
class PHONONCORE_EXPORT MediaProducerInterface
{
	public:
		virtual ~MediaProducerInterface() {}

		virtual bool addAudioPath(QObject*) = 0;
		virtual bool addVideoPath(QObject*) = 0;
		virtual void removeAudioPath(QObject*) = 0;
		virtual void removeVideoPath(QObject*) = 0;

		virtual QStringList availableAudioStreams() const = 0;
		virtual QStringList availableVideoStreams() const = 0;
		virtual QStringList availableSubtitleStreams() const = 0;

		virtual QString selectedAudioStream(const QObject*) const = 0;
		virtual QString selectedVideoStream(const QObject*) const = 0;
		virtual QString selectedSubtitleStream(const QObject*) const = 0;

		virtual void selectAudioStream(const QString&,const QObject*) = 0;
		virtual void selectVideoStream(const QString&,const QObject*) = 0;
		virtual void selectSubtitleStream(const QString&,const QObject*) = 0;

        /**
         * Requests the playback to start.
         *
         * The backend should react immediately
         * by either going into \ref PlayingState or \ref BufferingState if the
         * former is not possible.
         */
        virtual void play() = 0;

        /**
         * Requests the playback to pause.
         *
         * The backend should react as fast as possible. Go to \ref PausedState
         * as soon as playback is paused.
         */
        virtual void pause() = 0;

        /**
         * Requests the playback to be stopped.
         *
         * The backend should react as fast as possible. Go to \ref StoppedState
         * as soon as playback is stopped.
         *
         * A subsequent call to play() will start playback at the beginning of
         * the media.
         */
        virtual void stop() = 0;

        /**
         * Requests the playback to be seeked to the given time.
         *
         * The backend does not have to finish seeking while in this function
         * (i.e. the backend does not need to block the thread until the seek is
         * finished; even worse it might lead to deadlocks when using a
         * ByteStream which gets its data from the thread this function would
         * block).
         *
         * As soon as the seek is done the currentTime() function and
         * the tick() signal will report it.
         *
         * \param milliseconds The time where playback should seek to in
         * milliseconds.
         */
        virtual void seek(qint64 milliseconds) = 0;

		virtual qint32 tickInterval() const = 0;
		virtual void setTickInterval(qint32) = 0;

		virtual bool hasVideo() const = 0;
		virtual bool isSeekable() const = 0;
		virtual qint64 currentTime() const = 0;
		virtual Phonon::State state() const = 0;
};

} // namespace Phonon

Q_DECLARE_INTERFACE( Phonon::MediaProducerInterface, "org.kde.Phonon.MediaProducerInterface/0.1" )

#endif // PHONON_MEDIAPRODUCERINTERFACE_H
// vim: sw=4 ts=4 tw=80
