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
#ifndef Phonon_VIDEOPATH_H
#define Phonon_VIDEOPATH_H

#include "base.h"
#include "phonondefs.h"

namespace Phonon
{
	class VideoPathPrivate;
	class VideoEffect;
	class AbstractVideoOutput;

	namespace Ifaces
	{
		class VideoPath;
	}

	class PHONON_EXPORT VideoPath : public Base
	{
		friend class AbstractMediaProducer;
		friend class AbstractMediaProducerPrivate;
		Q_DECLARE_PRIVATE( VideoPath )
		Q_OBJECT
		PHONON_OBJECT( VideoPath )
		public:
			/**
			 * @fn VideoPath( QObject* parent = 0 );
			 */

			// Operations:
			bool insertEffect( VideoEffect* newEffect, VideoEffect* insertBefore = 0 );
			bool addOutput( AbstractVideoOutput* videoOutput );
			bool removeOutput( AbstractVideoOutput* videoOutput );
	};
} //namespace Phonon

// vim: sw=4 ts=4 tw=80 noet
#endif // Phonon_VIDEOPATH_H
