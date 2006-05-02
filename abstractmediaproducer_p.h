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

#ifndef ABSTRACTMEDIAPRODUCER_P_H
#define ABSTRACTMEDIAPRODUCER_P_H

#include "abstractmediaproducer.h"
#include "ifaces/abstractmediaproducer.h"
#include "base_p.h"
#include <QHash>
#include <QString>

namespace Phonon
{
	namespace Ifaces
	{
		class AudioPath;
		class VideoPath;
	}
class AbstractMediaProducerPrivate : public BasePrivate
{
	K_DECLARE_PUBLIC( AbstractMediaProducer )
	PHONON_PRIVATEABSTRACTCLASS( AbstractMediaProducer, Base )
	protected:
		AbstractMediaProducerPrivate()
			: state( Phonon::LoadingState )
			, currentTime( 0 )
			, tickInterval( 0 )
		{ }

		State state;
		long currentTime;
		long tickInterval;
		QList<VideoPath*> videoPaths;
		QList<AudioPath*> audioPaths;
		QHash<Ifaces::AudioPath*, QString> selectedAudioStream;
		QHash<Ifaces::VideoPath*, QString> selectedVideoStream;
		QHash<Ifaces::VideoPath*, QString> selectedSubtitleStream;
};
} //namespace Phonon

#endif // ABSTRACTMEDIAPRODUCER_P_H
// vim: sw=4 ts=4 tw=80
