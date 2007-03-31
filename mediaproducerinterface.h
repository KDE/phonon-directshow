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
 * The backend implementation has to provide two signals, that are not defined
 * in this interface:
 * <ul>
 * <li>\anchor phonon_MediaProducerInterface_stateChanged
 * <b>void stateChanged(\ref Phonon::State newstate, \ref Phonon::State oldstate)</b>
 *
 * Emitted when the state of the MediaObject has changed.
 * In case you're not interested in the old state you can also
 * connect to a slot that only has one State argument.
 *
 * \param newstate The state the Player is in now.
 * \param oldstate The state the Player was in before.
 * </li>
 * <li>\anchor phonon_MediaProducerInterface_tick
 * <b>void tick(qint64 time)</b>
 *
 * This signal gets emitted every tickInterval milliseconds.
 *
 * \param time The position of the media file in milliseconds.
 *
 * \see setTickInterval()
 * \see tickInterval()
 * </li>
 * </ul>
 *
 * \ingroup Backend
 * \author Matthias Kretz <kretz@kde.org>
 * \see AbstractMediaProducer
 */
class PHONONCORE_EXPORT MediaProducerInterface
{
    public:
        virtual ~MediaProducerInterface() {}

        /**
         * Adds an AudioPath object to tell where to send the audio data.
         *
         * The frontend class ensures that the \p audioPath is unique,
         * meaning the AudioPath object has not been added to this object.
         */
        virtual bool addAudioPath(QObject *audioPath) = 0;
        /**
         * Adds a VideoPath object to tell where to send the video data (and
         * render the subtitle if one is selected).
         *
         * The frontend class ensures that the \p videoPath is unique,
         * meaning the VideoPath object has not been added to this object.
         */
        virtual bool addVideoPath(QObject *videoPath) = 0;
        /**
         * Removes the AudioPath object from the flow graph. This function will
         * only be called with a valid \p audioPath, meaning that the path has been
         * added to this object before.
         */
        virtual void removeAudioPath(QObject *audioPath) = 0;
        /**
         * Removes the VideoPath object from the flow graph. This function will
         * only be called with a valid \p videoPath, meaning that the path has been
         * added to this object before.
         */
        virtual void removeVideoPath(QObject *videoPath) = 0;

        /**
         * Lists the available audio streams the media provides. The strings are
         * what is shown to the user and often just consist of the stream name
         * encoded into the media file.
         *
         * Be prepared to change the audio stream when \ref
         * phonon_AbstractMediaProducer_selectAudioStream is called with one of the
         * strings of this list. If the media has multiple audio streams but the
         * backend cannot switch then the list of available audio streams should be
         * empty.
         *
         * \returns A list of names for the available audio streams.
         */
        virtual QStringList availableAudioStreams() const = 0;
        /**
         * Lists the available video streams the media provides. The strings are
         * what is shown to the user and often just consist of the stream name
         * encoded into the media file.
         *
         * Be prepared to change the video stream when \ref
         * phonon_AbstractMediaProducer_selectVideoStream is called with one of the
         * strings of this list. If the media has multiple video streams but the
         * backend cannot switch then the list of available video streams should be
         * empty.
         *
         * \returns A list of names for the available video streams.
         */
        virtual QStringList availableVideoStreams() const = 0;
        /**
         * Lists the available subtitle streams the media provides. The strings are
         * what is shown to the user and often just consist of the stream name
         * encoded into the media file.
         *
         * Be prepared to change the subtitle stream when \ref
         * phonon_AbstractMediaProducer_selectSubtitleStream is called with one of the
         * strings of this list. If the media has multiple subtitle streams but the
         * backend cannot switch then the list of available subtitle streams should be
         * empty.
         *
         * \returns A list of names for the available subtitle streams.
         */
        virtual QStringList availableSubtitleStreams() const = 0;

        /**
         * Returns the selected audio stream for the given AudioPath object.
         */
        virtual QString selectedAudioStream(const QObject *audioPath) const = 0;
        /**
         * Returns the selected video stream for the given VideoPath object.
         */
        virtual QString selectedVideoStream(const QObject *videoPath) const = 0;
        /**
         * Returns the selected subtitle stream for the given VideoPath object.
         */
        virtual QString selectedSubtitleStream(const QObject *videoPath) const = 0;

        /**
         * Selects one audio stream for the selected AudioPath object.
         *
         * \param audioPath If \p 0 the audio stream should be used for all
         * connected AudioPath objects. Else the audio stream selection should only
         * be made for the one specified AudioPath. This way the user can request
         * different audio streams for different audio paths/outputs.
         */
        virtual void selectAudioStream(const QString &streamName,const QObject *audioPath) = 0;
        /**
         * Selects one video stream for the selected VideoPath object.
         *
         * \param videoPath If \p 0 the video stream should be used for all
         * connected VideoPath objects. Else the video stream selection should only
         * be made for the one specified VideoPath. This way the user can request
         * different video streams for different video paths/outputs.
         */
        virtual void selectVideoStream(const QString &streamName,const QObject *videoPath) = 0;
        /**
         * Selects one subtitle stream for the selected VideoPath object. By
         * default normally no subtitle is selected, but depending on the media
         * this may be different.
         *
         * \param videoPath If \p 0 the video stream should be used for all
         * connected VideoPath objects. Else the video stream selection should only
         * be made for the one specified VideoPath. This way the user can request
         * different video streams for different video paths/outputs.
         */
        virtual void selectSubtitleStream(const QString &streamName,const QObject *videoPath) = 0;

        /**
         * Requests the playback to start.
         *
         * This method is only called if the state transition to \ref PlayingState is possible.
         *
         * The backend should react immediately
         * by either going into \ref PlayingState or \ref BufferingState if the
         * former is not possible.
         */
        virtual void play() = 0;

        /**
         * Requests the playback to pause.
         *
         * This method is only called if the state transition to \ref PausedState is possible.
         *
         * The backend should react as fast as possible. Go to \ref PausedState
         * as soon as playback is paused.
         */
        virtual void pause() = 0;

        /**
         * Requests the playback to be stopped.
         *
         * This method is only called if the state transition to \ref StoppedState is possible.
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

        /**
         * Return the time interval in milliseconds between two ticks.
         *
         * \returns Returns the tick interval that it was set to (might not
         * be the same as you asked for).
         */
        virtual qint32 tickInterval() const = 0;
        /**
         * Change the interval the tick signal is emitted. If you set \p
         * interval to 0 the signal gets disabled.
         *
         * \param interval tick interval in milliseconds
         *
         * \returns Returns the tick interval that it was set to (might not
         *          be the same as you asked for).
         */
        virtual void setTickInterval(qint32 interval) = 0;

        /**
         * Check whether the media data includes a video stream.
         *
         * \return returns \p true if the media contains video data
         */
        virtual bool hasVideo() const = 0;
        /**
         * If the current media may be seeked returns true.
         *
         * \returns whether the current media may be seeked.
         */
        virtual bool isSeekable() const = 0;
        /**
         * Get the current time (in milliseconds) of the file currently being played.
         */
        virtual qint64 currentTime() const = 0;
        /**
         * Get the current state.
         */
        virtual Phonon::State state() const = 0;

        /**
         * A translated string describing the error.
         */
        virtual QString errorString() const = 0;

        /**
         * Tells your program what to do about the error.
         *
         * \see Phonon::ErrorType
         */
        virtual Phonon::ErrorType errorType() const = 0;
};

} // namespace Phonon

Q_DECLARE_INTERFACE( Phonon::MediaProducerInterface, "org.kde.Phonon.MediaProducerInterface/0.2" )

#endif // PHONON_MEDIAPRODUCERINTERFACE_H
// vim: sw=4 ts=4 tw=80
