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
#include "videopath.h"
#include "videopath_p.h"
#include "ifaces/videopath.h"
#include "factory.h"

#include "videoeffect.h"
#include "abstractvideooutput.h"

namespace Phonon
{
PHONON_OBJECT_IMPL( VideoPath )

bool VideoPath::insertEffect( VideoEffect* newEffect, VideoEffect* insertBefore )
{
	return iface()->insertEffect( newEffect->iface(), insertBefore->iface() );
}

bool VideoPath::addOutput( AbstractVideoOutput* videoOutput )
{
	return iface()->addOutput( videoOutput->iface() );
}

bool VideoPath::removeOutput( AbstractVideoOutput* videoOutput )
{
	return iface()->removeOutput( videoOutput->iface() );
}

bool VideoPathPrivate::aboutToDeleteIface()
{
	return true;
}

void VideoPath::setupIface()
{
	Q_D( VideoPath );
	Q_ASSERT( d->iface() );

	// set up attributes
}

} //namespace Phonon

#include "videopath.moc"

// vim: sw=4 ts=4 tw=80 noet
