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
#ifndef Kdem2m_AUDIOEFFECT_H
#define Kdem2m_AUDIOEFFECT_H

#include <QObject>
#include <kdelibs_export.h>
#include "kdem2mdefs.h"

class QString;
class QStringList;

namespace Kdem2m
{
	namespace Ifaces
	{
		class AudioEffect;
	}

	/**
	 * @author Matthias Kretz <kretz@kde.org>
	 * @since 4.0
	 */
	class KDEM2M_EXPORT AudioEffect : public QObject
	{
		friend class AudioPath;
		Q_OBJECT
		KDEM2M_OBJECT( AudioEffect )
		public:
			QString type() const;

		public slots:
			void setType( const QString& );
	};
} //namespace Kdem2m

// vim: sw=4 ts=4 tw=80 noet
#endif // Kdem2m_AUDIOEFFECT_H
