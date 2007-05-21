/*  This file is part of the KDE project
    Copyright (C) 2006-2007 Matthias Kretz <kretz@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.

*/

#ifndef MEDIAOBJECTTEST_H
#define MEDIAOBJECTTEST_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtTest/QSignalSpy>

#include <phonon/bytestream.h>
#include <phonon/audiopath.h>
#include <phonon/audiooutput.h>

#include <kio/jobclasses.h>

class ByteStreamTest : public QObject
{
    Q_OBJECT
    public:
        ByteStreamTest()
            : m_job(0),
            m_media(0),
            m_stateChangedSignalSpy(0),
            m_audioPath(0),
            m_audioOutput(0)
        {}

    Q_SIGNALS:
        void continueTestPlayOnFinish();

    private Q_SLOTS:
        void init();
        void cleanup();

        void initTestCase();
        void checkForDefaults();

        // state change tests
        void stopToStop();
        void stopToPause();
        void stopToPlay();
        void playToPlay();
        void playToPause();
        void playToStop();
        void pauseToPause();
        void pauseToPlay();
        void pauseToStop();

        void testTickSignal();
        void testSeek();
        void testAboutToFinish();

        void cleanupTestCase();

    protected Q_SLOTS:
        void kioTotalSize(KJob *,qulonglong size);
        void kioData(KIO::Job *,const QByteArray &);
        void kioResult(KJob *);
        void kioSeekDone(KIO::Job *, KIO::filesize_t);
        void needData();
        void enoughData();
        void seekStream(qint64 offset);
        void kioJobOpen(KIO::Job *);

    private:
        void setMedia();
        void addPaths();
        void initOutput();

        void initByteStream();

        void startPlayback(Phonon::State currentState = Phonon::StoppedState);
        void stopPlayback(Phonon::State currentState);
        void pausePlayback();
        void waitForSignal(QObject *obj, const char *signalName, int timeout = 0);
        void testOneSeek(qint64 seekTo);

        QUrl m_url;
        KIO::Job *m_job;
        Phonon::ByteStream *m_media;
        QSignalSpy *m_stateChangedSignalSpy;
        Phonon::AudioPath *m_audioPath;
        Phonon::AudioOutput *m_audioOutput;
        bool m_endOfDataSent;
        bool m_reading;
        bool m_seeking;
};

// vim: sw=4 ts=4
#endif // MEDIAOBJECTTEST_H
