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
#ifndef Kdem2m_AUDIOOUTPUT_H
#define Kdem2m_AUDIOOUTPUT_H

#include "abstractaudiooutput.h"
#include "mixeriface.h"
#include <kdelibs_export.h>
#include "kdem2mdefs.h"

class QString;

namespace Kdem2m
{
	namespace Ifaces
	{
		class AudioOutput;
	}

	class KDEM2M_EXPORT AudioOutput : public AbstractAudioOutput, public MixerIface
	{
		Q_OBJECT
		KDEM2M_HEIR( AudioOutput, AbstractAudioOutput )
		public:
			// Attributes Getters:
			QString name() const;
			float volume() const;

		public slots:
			// Attributes Setters:
			void setName( const QString& newName );
			void setVolume( float newVolume );

		signals:
			void volumeChanged( float newVolume );
	};
} //namespace Kdem2m

// vim: sw=4 ts=4 tw=80 noet
#endif // Kdem2m_AUDIOOUTPUT_H
