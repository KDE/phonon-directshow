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

#ifndef PHONON_MEDIACONTROLLER_H
#define PHONON_MEDIACONTROLLER_H

#include "phonon/phonon_export.h"
#include "phonon/objectdescription.h"

#include <QtCore/QObject>

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE

namespace Phonon
{
class MediaControllerPrivate;
class MediaObject;

/** \class MediaController mediacontroller.h Phonon/MediaController
 * \brief Controls optional features of a media file/device like title, chapter, angle.
 *
 * \ingroup Playback
 * \author Matthias Kretz <kretz@kde.org>
 */
class PHONON_EXPORT MediaController : public QObject
{
    Q_OBJECT
    Q_FLAGS(Features)
    public:
        enum Feature {
            Angles = 1,
            Chapters = 2,
            Titles = 4
        };
        Q_DECLARE_FLAGS(Features, Feature)

        MediaController(MediaObject *parent);
        ~MediaController();

        Features supportedFeatures() const;

        int availableAngles() const;
        int currentAngle() const;

        int availableChapters() const;
        int currentChapter() const;

        int availableTitles() const;
        int currentTitle() const;

        bool autoplayTitles() const;

        /**
         * Returns the selected audio stream.
         *
         * \see availableAudioStreams
         * \see setCurrentAudioStream
         */
        AudioStreamDescription currentAudioStream(const QObject *audioPath = 0) const;

        /**
         * Returns the selected subtitle stream.
         *
         * \see availableSubtitleStreams
         * \see setCurrentSubtitleStream
         */
        SubtitleStreamDescription currentSubtitleStream(const QObject *videoPath = 0) const;

        /**
         * Returns the audio streams that can be selected by the user. The
         * strings can directly be used in the user interface.
         *
         * \see selectedAudioStream
         * \see setCurrentAudioStream
         */
        QList<AudioStreamDescription> availableAudioStreams(const QObject *audioPath = 0) const;

        /**
         * Returns the subtitle streams that can be selected by the user. The
         * strings can directly be used in the user interface.
         *
         * \see selectedSubtitleStream
         * \see setCurrentSubtitleStream
         */
        QList<SubtitleStreamDescription> availableSubtitleStreams(const QObject *videoPath = 0) const;

        /**
         * Selects an audio stream from the media.
         *
         * Some media formats allow multiple audio streams to be stored in
         * the same file. Normally only one should be played back.
         *
         * \param stream Description of an audio stream
         *
         * \see availableAudioStreams()
         * \see currentAudioStream()
         */
        void setCurrentAudioStream(const Phonon::AudioStreamDescription &stream, const QObject* audioPath = 0);

        /**
         * Selects a subtitle stream from the media.
         *
         * Some media formats allow multiple subtitle streams to be stored in
         * the same file. Normally only one should be displayed.
         *
         * \param stream description of a subtitle stream
         *
         * \see availableSubtitleStreams()
         * \see currentSubtitleStream()
         */
        void setCurrentSubtitleStream(const Phonon::SubtitleStreamDescription &stream, const QObject* videoPath = 0);

    public Q_SLOTS:
        void setCurrentAngle(int angleNumber);
        void setCurrentChapter(int chapterNumber);

        /**
         * Skips to the given title \p titleNumber.
         *
         * If it was playing before the title change it will start playback on the new title if
         * autoplayTitles is enabled.
         */
        void setCurrentTitle(int titleNumber);
        void setAutoplayTitles(bool);

        /**
         * Skips to the next title.
         *
         * If it was playing before the title change it will start playback on the next title if
         * autoplayTitles is enabled.
         */
        void nextTitle();

        /**
         * Skips to the previous title.
         *
         * If it was playing before the title change it will start playback on the previous title if
         * autoplayTitles is enabled.
         */
        void previousTitle();

    Q_SIGNALS:
        void availableAnglesChanged(int availableAngles);
        void angleChanged(int angleNumber);
        void availableChaptersChanged(int availableChapters);
        void chapterChanged(int chapterNumber);
        void availableTitlesChanged(int availableTitles);
        void titleChanged(int titleNumber);

    protected:
        MediaControllerPrivate *const d;
};

} // namespace Phonon

Q_DECLARE_OPERATORS_FOR_FLAGS(Phonon::MediaController::Features)

QT_END_NAMESPACE
QT_END_HEADER

#endif // PHONON_MEDIACONTROLLER_H
