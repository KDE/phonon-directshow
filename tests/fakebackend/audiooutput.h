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
#ifndef Phonon_FAKE_AUDIOOUTPUT_H
#define Phonon_FAKE_AUDIOOUTPUT_H

#include "abstractaudiooutput.h"
#include <QFile>

namespace Phonon
{
namespace Fake
{
	class AudioOutput : public AbstractAudioOutput
	{
		Q_OBJECT
		public:
			AudioOutput( QObject* parent );
			~AudioOutput();

		public Q_SLOTS:
			// Attributes Getters:
			float volume() const;
			int outputDevice() const;

			// Attributes Setters:
			void setVolume( float newVolume );
			void setOutputDevice( int newDevice );

		public:
			virtual void processBuffer( const QVector<float>& buffer );

			void openDevice();
			void closeDevice();

		Q_SIGNALS:
			void volumeChanged( float newVolume );

		private:
			float m_volume;
			int m_device;
			QFile m_dsp;
	};
}} //namespace Phonon::Fake

// vim: sw=4 ts=4 tw=80
#endif // Phonon_FAKE_AUDIOOUTPUT_H
