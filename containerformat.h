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

#ifndef PHONON_CONTAINERFORMAT_H
#define PHONON_CONTAINERFORMAT_H

#include "namedescriptiontuple.h"

namespace Phonon
{

class ContainerFormatPrivate;

/**
 * \author Matthias Kretz <kretz@kde.org>
 * \see Visualization
 */
class PHONONCORE_EXPORT ContainerFormat : public NameDescriptionTuple
{
	Q_DECLARE_PRIVATE( ContainerFormat )
	public:
		ContainerFormat();
		ContainerFormat( const ContainerFormat& visualizationEffect );
		ContainerFormat& operator=( const ContainerFormat& visualizationEffect );
		bool operator==( const ContainerFormat& visualizationEffect ) const;
		static ContainerFormat fromIndex( int index );

	protected:
		/**
		 * \internal
		 * Creates new (valid) description.
		 */
		ContainerFormat( int index, const QString& name, const QString& description );
};

}

#endif // PHONON_CONTAINERFORMAT_H
// vim: sw=4 ts=4 noet tw=80
