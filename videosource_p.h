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

#ifndef PHONON_VIDEOCAPTURESOURCE_P_H
#define PHONON_VIDEOCAPTURESOURCE_P_H

#include "namedescriptiontuple_p.h"

namespace Phonon
{
	class VideoSourcePrivate : NameDescriptionTuplePrivate
	{
		Q_DECLARE_PUBLIC( VideoSource )
		protected:
			VideoSourcePrivate()
				: NameDescriptionTuplePrivate()
				, audioIndex( -1 )
			{
			}

			VideoSourcePrivate( const VideoSourcePrivate* cpy )
				: NameDescriptionTuplePrivate( cpy )
				, audioIndex( cpy->audioIndex )
			{
			}
			
			VideoSourcePrivate& operator=( const VideoSourcePrivate& rhs )
			{
				audioIndex = rhs.audioIndex;
				NameDescriptionTuplePrivate::operator=( rhs );
				return *this;
			}

			bool operator==( const VideoSourcePrivate& rhs ) const
			{
				return NameDescriptionTuplePrivate::operator==( rhs ) && audioIndex == rhs.audioIndex;
			}

			int audioIndex;
	};
} // namespace Phonon

#endif // PHONON_VIDEOCAPTURESOURCE_P_H
// vim: sw=4 ts=4 noet tw=80
