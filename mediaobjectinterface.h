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

#ifndef PHONON_MEDIAOBJECTINTERFACE_H
#define PHONON_MEDIAOBJECTINTERFACE_H

#include <kdelibs_export.h>

namespace Phonon
{
class PHONONCORE_EXPORT MediaObjectInterface
{
	public:
		virtual ~MediaObjectInterface() {}
		virtual qint64 totalTime() const = 0;
		virtual KUrl url() const = 0;
		virtual void setUrl( const KUrl& url ) = 0;
};
}

Q_DECLARE_INTERFACE( Phonon::MediaObjectInterface, "org.kde.Phonon.MediaObjectInterface/0.1" )

#endif // PHONON_MEDIAOBJECTINTERFACE_H
// vim: sw=4 ts=4 tw=80
