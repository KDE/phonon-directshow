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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.

*/

#ifndef Phonon_TEMPLATE_H
#define Phonon_TEMPLATE_H

#include <QObject>
#include <kdelibs_export.h>
#include "phonondefs.h"

namespace Phonon
{
	namespace Ifaces
	{
		class Template;
	}

	/**
	 * \author Matthias Kretz <kretz@kde.org>
	 * \since 4.0
	 */
	class PHONON_EXPORT Template : public QObject
	{
		Q_OBJECT
		PHONON_OBJECT( Template )
	};
} //namespace Phonon

// vim: sw=4 ts=4 tw=80 noet
#endif // Phonon_TEMPLATE_H
