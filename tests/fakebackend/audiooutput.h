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
#ifndef Kdem2m_FAKE_AUDIOOUTPUT_H
#define Kdem2m_FAKE_AUDIOOUTPUT_H

#include "abstractaudiooutput.h"
#include "../../ifaces/audiooutput.h"

namespace Kdem2m
{
namespace Fake
{
	class AudioOutput : virtual public AbstractAudioOutput, virtual public Ifaces::AudioOutput
	{
		Q_OBJECT
		public:
			AudioOutput( QObject* parent );
			virtual ~AudioOutput();

			// Attributes Getters:
			virtual QString name() const;
			virtual float volume() const;

			// Attributes Setters:
			virtual QString setName( const QString& newName );
			virtual float setVolume( float newVolume );

		signals:
			void volumeChanged( float newVolume );

		private:
			float m_volume;
			QString m_name;
	};
}} //namespace Kdem2m::Fake

// vim: sw=4 ts=4 tw=80 noet
#endif // Kdem2m_FAKE_AUDIOOUTPUT_H
