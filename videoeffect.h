/*  This file is part of the KDE project
    Copyright (C) 2005-2006 Matthias Kretz <kretz@kde.org>

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
#ifndef Phonon_VIDEOEFFECT_H
#define Phonon_VIDEOEFFECT_H

#include "base.h"
#include "phonondefs.h"

class QString;

namespace Phonon
{
	class VideoEffectPrivate;
	namespace Ifaces
	{
		class VideoEffect;
	}

	class PHONON_EXPORT VideoEffect : public Base
	{
		friend class VideoPath;
		friend class VideoPathPrivate;
		Q_DECLARE_PRIVATE( VideoEffect )
		Q_OBJECT
		PHONON_OBJECT( VideoEffect )
		public:
			QString type() const;

		public Q_SLOTS:
			void setType( const QString& );
	};
} //namespace Phonon

// vim: sw=4 ts=4 tw=80 noet
#endif // Phonon_VIDEOEFFECT_H
