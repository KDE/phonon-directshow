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
#ifndef Phonon_IFACES_AUDIOEFFECT_H
#define Phonon_IFACES_AUDIOEFFECT_H

#include "base.h"

class QString;
class QStringList;

namespace Phonon
{
namespace Ifaces
{
	/**
	 * \author Matthias Kretz <kretz@kde.org>
	 */
	class AudioEffect : virtual public Base
	{
		public:
			// Attributes Getters:
			virtual QString type() const = 0;

			// Attributes Setters:
			virtual void setType( const QString& type ) = 0;
	};
}} //namespace Phonon::Ifaces

// vim: sw=4 ts=4 tw=80 noet
#endif // Phonon_IFACES_AUDIOEFFECT_H
